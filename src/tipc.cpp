#include <iostream>
#include <stdlib.h>

#include "antlr4-runtime.h"
#include "TIPLexer.h"
#include "TIPParser.h"
#include "TIPtreeBuild.h"
#include "TIPtreeGen.h"
#include "llvm/Support/CommandLine.h"
 
using namespace std;
using namespace antlr4;
using namespace TIPtree;
using namespace llvm;

static bool pretty = false;
static bool disableOpt = true;
static std::string sourceFile;
/*
static cl::opt<bool> pretty("p", cl::desc("pretty print"));
static cl::opt<bool> pretty("l", cl::desc("pretty print with line numbers"));
static cl::opt<bool> disableOpt("d", cl::desc("disable bitcode optimization"));
static cl::opt<std::string> sourceFile(cl::Positional, 
                                       cl::desc("<input file>"), 
                                       cl::Required);
*/

int main(int argc, const char* argv[]) {
/*
    cl::ParseCommandLineOptions(argc, argv, "tipc - TIP llvm compiler\n");
*/
    sourceFile = argv[1];

    std::ifstream stream;
    stream.open(sourceFile);
    
    ANTLRInputStream input(stream);
    TIPLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    TIPParser parser(&tokens);    
 
    TIPParser::ProgramContext *tree = parser.program();
 
    TIPtreeBuild tb(&parser);
    auto ast = tb.build(tree);

    if (pretty) {
       std::cout << ast->print("  ", true);
    } else {
       auto theModule = ast->codegen(sourceFile);

       if (!disableOpt) {
           // Create a pass manager to simplify generated module
           auto TheFPM = 
              llvm::make_unique<legacy::FunctionPassManager>(theModule.get());

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
       ToolOutputFile result(sourceFile+".bc", ec, sys::fs::F_None);
       WriteBitcodeToFile(*theModule, result.os());
       result.keep();
    }
}
