#include <iostream>
#include <stdlib.h>

#include "antlr4-runtime.h"
#include "TIPLexer.h"
#include "TIPParser.h"

#include "ASTBuilder.h"
#include "PrettyPrinter.h"
#include "SymbolTable.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"

using namespace std;
using namespace antlr4;
using namespace llvm;
using namespace AST;

static cl::OptionCategory TIPcat("tipc Options",
                                 "Options for controlling the TIP compilation process.");
static cl::opt<bool> ppretty("p", cl::desc("pretty print"), cl::cat(TIPcat));
static cl::opt<bool> psym("s", cl::desc("print symbol table"), cl::cat(TIPcat));
static cl::opt<bool> disopt("d", cl::desc("disable bitcode optimization"), cl::cat(TIPcat));
static cl::opt<std::string> sourceFile(cl::Positional,
                                       cl::desc("<tip source file>"),
                                       cl::Required, cl::cat(TIPcat));

// Handle parse errors
class ErrorListener : public BaseErrorListener {
  std::shared_ptr<bool> error;
public:
  ErrorListener(std::shared_ptr<bool> e) : error(e) {}

  void syntaxError(Recognizer *recognizer, Token *offendingSymbol,
                   size_t line, size_t charPositionInLine,
                   const std::string &msg, std::exception_ptr e) {
    *error = true;
  }
};

int main(int argc, const char *argv[]) {
  cl::HideUnrelatedOptions(TIPcat); // suppress non TIP options
  cl::ParseCommandLineOptions(argc, argv, "tipc - a TIP to llvm compiler\n");

  std::ifstream stream;
  stream.open(sourceFile);

  ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);

  std::shared_ptr<bool> parseError = std::make_shared<bool>(false);
  ErrorListener errorListener(parseError);

  // Add error listeners
  lexer.addErrorListener(&errorListener);
  parser.addErrorListener(&errorListener);

  TIPParser::ProgramContext *tree = parser.program();

  if (*parseError) {
    cerr << "tipc parse error\n";
    exit (EXIT_FAILURE);
  }

  ASTBuilder ab(&parser);
  // Build AST
  if (auto maybeAST = ab.build(tree); maybeAST) {
    auto ast = std::move(maybeAST.value());

    // Build Symbol Table
    if (auto maybeSymTable = SymbolTable::build(ast.get(), std::cerr); maybeSymTable) {
      auto symbols = std::move(maybeSymTable.value());

      if (ppretty) {
        PrettyPrinter::print(ast.get(), std::cout, ' ', 2);
        if (psym) { 
          SymbolTable::print(symbols.get(), std::cout);
        }
      } else {
        auto theModule = ast->codegen(sourceFile);

        if (!disopt) {
          // Create a pass manager to simplify generated module
          auto TheFPM =
              std::make_unique<legacy::FunctionPassManager>(theModule.get());

          // Promote allocas to registers.
          TheFPM->add(createPromoteMemoryToRegisterPass());
          // Do simple "peephole" optimizations
          TheFPM->add(createInstructionCombiningPass());
          // Reassociate expressions.
          TheFPM->add(createReassociatePass());
          // Eliminate Common SubExpressions.
          TheFPM->add(createGVNPass());
          // Simplify the control flow graph (deleting unreachable blocks, etc).
          TheFPM->add(createCFGSimplificationPass());
          TheFPM->doInitialization();
  
          // run simplification pass on each function
          for (auto &fun : theModule->getFunctionList()) {
            TheFPM->run(fun);
          }
        }

        std::error_code ec;
        ToolOutputFile result(sourceFile + ".bc", ec, sys::fs::F_None);
        WriteBitcodeToFile(*theModule, result.os());
        result.keep();
      }
    } else {
      cerr << "tipc Symbol Table build error\n";
      exit (EXIT_FAILURE);
    }

  } else {
    cerr << "tipc AST build error\n";
    exit (EXIT_FAILURE);
  }
}
