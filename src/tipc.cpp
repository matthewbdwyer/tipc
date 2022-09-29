#include "FrontEnd.h"
#include "SemanticAnalysis.h"
#include "CodeGenerator.h"
#include "Optimizer.h"
#include "ParseError.h"
#include "InternalError.h"
#include "SemanticError.h"
#include "llvm/Support/CommandLine.h"
#include "loguru.hpp"

#include <fstream>

using namespace llvm;
using namespace std;

static cl::OptionCategory TIPcat("tipc Options","Options for controlling the TIP compilation process.");
static cl::opt<bool> ppretty("pp", cl::desc("pretty print"), cl::cat(TIPcat));
static cl::opt<bool> psym("ps", cl::desc("print symbols"), cl::cat(TIPcat));
static cl::opt<bool> ptypes("pt", cl::desc("print symbols with types (supercedes --ps)"), cl::cat(TIPcat));
static cl::opt<bool> disopt("do", cl::desc("disable bitcode optimization"), cl::cat(TIPcat));
static cl::opt<int> debug("verbose", cl::desc("enable log messages (Levels 1-3) \n Level 1 - Basic logging for every phase.\n Level 2 - Level 1 and type constraints being unified.\n Level 3 - Level 2 and union-find solving steps."), cl::cat(TIPcat));
static cl::opt<bool> emitHrAsm("asm",
                           cl::desc("emit human-readable LLVM assembly language"),
                           cl::cat(TIPcat));
static cl::opt<std::string> cgFile("pcg", 
                         cl::value_desc("call graph output file"),
                         cl::desc("print call graph to a file in dot syntax"), 
                         cl::cat(TIPcat));
static cl::opt<std::string> astFile("pa",
                                 cl::value_desc("AST output file"),
                                 cl::desc("print AST to a file in dot syntax"),
                                 cl::cat(TIPcat));
static cl::opt<std::string> logfile("log",
                                   cl::value_desc("logfile"),
                                   cl::desc("log all messages to logfile (enables --verbose 3)"),
                                   cl::cat(TIPcat));
static cl::opt<std::string> sourceFile(cl::Positional,
                                       cl::desc("<tip source file>"),
                                       cl::Required,
                                       cl::cat(TIPcat));
static cl::opt<std::string> outputfile("o",
                                    cl::value_desc("outputfile"),
                                    cl::desc("write output to <outputfile>"),
                                    cl::cat(TIPcat));

/*! \brief tipc driver.
 * 
 * This function is the entry point for tipc.   It handles command line parsing
 * using LLVM CommandLine support.  It runs the phases of the compiler in sequence.
 * If an error is detected, via an exception, it reports the error and exits.  
 * If there is no error, then the LLVM bitcode is emitted to a file whose name
 * is the providvvved source file suffixed by ".bc".
 */
int main(int argc, char *argv[]) {
  cl::HideUnrelatedOptions(TIPcat);
  cl::ParseCommandLineOptions(argc, argv, "tipc - a TIP to llvm compiler\n");

  loguru::g_preamble = false;
  bool logging = !logfile.getValue().empty();
  if(debug || logging) {
    loguru::g_preamble = true;
    loguru::g_preamble_date = false;
    loguru::g_preamble_time = false;
    loguru::g_preamble_uptime = false;
    loguru::g_preamble_thread = false;
    loguru::init(argc, argv);
    loguru::g_stderr_verbosity = logging ? loguru::Verbosity_OFF : debug;
    if (logging) {
      loguru::add_file(logfile.getValue().c_str(), loguru::Append, loguru::Verbosity_MAX);
    }
  }

  std::ifstream stream;
  stream.open(sourceFile);
  if(!stream.good()) {
    LOG_S(ERROR) << "tipc: error: no such file: '" << sourceFile << "'";
    exit(1);
  }

  /*
   * Program representations, e.g., ast, analysis results, etc., are
   * represented using smart pointers.  The driver "owns" this data and
   * it permits other components to read the contents by passing
   * the underlying pointer, i.e., via a call to get().
   */
  try {
    std::shared_ptr<ASTProgram> ast = std::move(FrontEnd::parse(stream));

    try {
      auto analysisResults = SemanticAnalysis::analyze(ast.get());


      if (ppretty) {
        FrontEnd::prettyprint(ast.get(), std::cout);
      }

      if (ptypes) {
        analysisResults->getTypeResults()->print(std::cout);
      } else if (psym) {
        analysisResults->getSymbolTable()->print(std::cout);
      }

      bool printCG = !cgFile.getValue().empty();
      if(printCG) {
        std::ofstream cgStream;
        cgStream.open(cgFile);
        if(!cgStream.good()) {
          LOG_S(ERROR) << "tipc: error: failed to open '" << cgFile << "' for writing";
          exit(1);
        }

        analysisResults->getCallGraph()->print(cgStream);
      }

      auto llvmModule = CodeGenerator::generate(ast.get(), analysisResults.get(), sourceFile);

      if (!disopt) {
        Optimizer::optimize(llvmModule.get());
      }

      if(emitHrAsm) {
        CodeGenerator::emitHumanReadableAssembly(llvmModule.get(), outputfile);
      } else {
        CodeGenerator::emit(llvmModule.get(), outputfile);
      }

      bool printAST = !astFile.getValue().empty();
      if(printAST) {
        std::ofstream astStream;
        astStream.open(astFile);
        if(!astStream.good()) {
          LOG_S(ERROR) << "tipc: error: failed to open '" << astFile << "' for writing";
          exit(1);
        }

        FrontEnd::astVisualize(ast, astStream);
      }

    } catch (SemanticError& e) {
      LOG_S(ERROR) << "tipc: " << e.what();
      LOG_S(ERROR) << "tipc: semantic error";
      exit (EXIT_FAILURE);
    } catch (InternalError& e) { // LCOV_EXCL_LINE
      /* Internal errors should never happen, but we have logic to catch 
       * them just in case.  We do not want to count these lines toward 
       * coverage goals since a working compiler will never cover these.
       */
      LOG_S(ERROR) << "tipc: " << e.what(); // LCOV_EXCL_LINE
      LOG_S(ERROR) << "tipc: internal error"; // LCOV_EXCL_LINE
      exit (EXIT_FAILURE); // LCOV_EXCL_LINE
    }
  } catch (ParseError& e) {
    LOG_S(ERROR) << "tipc: " << e.what();
    LOG_S(ERROR) << "tipc: parse error";
    exit (EXIT_FAILURE);
  }  // LCOV_EXCL_LINE
}  // LCOV_EXCL_LINE
