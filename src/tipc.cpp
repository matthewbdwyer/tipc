#include "FrontEnd.h"
#include "SemanticAnalysis.h"
#include "CodeGenerator.h"
#include "Optimizer.h"
#include "ParseError.h"
#include "SemanticError.h"
#include "llvm/Support/CommandLine.h"
#include "loguru.hpp"
#include <fstream>

using namespace llvm;
using namespace std;

static cl::OptionCategory TIPcat("tipc Options",
                                 "Options for controlling the TIP compilation process.");
static cl::opt<bool> ppretty("pp", cl::desc("pretty print"), cl::cat(TIPcat));
static cl::opt<bool> psym("ps", cl::desc("print symbols"), cl::cat(TIPcat));
static cl::opt<bool> ptypes("pt", cl::desc("print symbols with types (supercedes --ps)"), cl::cat(TIPcat));
static cl::opt<bool> disopt("do", cl::desc("disable bitcode optimization"), cl::cat(TIPcat));
static cl::opt<bool> debug("verbose", cl::desc("enable log messages"), cl::cat(TIPcat));
static cl::opt<bool> emitHrAsm("asm",
                           cl::desc("emit human-readable LLVM assembly language instead of llvm bitcode"),
                           cl::cat(TIPcat));
static cl::opt<std::string> logfile("log",
                                   cl::value_desc("logfile"),
                                   cl::desc("log all messages to logfile (enables --verbose)"),
                                   cl::cat(TIPcat));
static cl::opt<std::string> sourceFile(cl::Positional,
                                       cl::desc("<tip source file>"),
                                       cl::Required,
                                       cl::cat(TIPcat));

/*! \brief tipc driver.
 * 
 * This function is the entry point for tipc.   It handles command line parsing
 * using LLVM CommandLine support.  It runs the phases of the compiler in sequence.
 * If an error is detected, via an exception, it reports the error and exits.  
 * If there is no error, then the LLVM bitcode is emitted to a file whose name
 * is the provided source file suffixed by ".bc".
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
    loguru::g_stderr_verbosity = logging ? loguru::Verbosity_OFF : 1;
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
    auto ast = FrontEnd::parse(stream);

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
      auto llvmModule = CodeGenerator::generate(ast.get(), analysisResults.get(), sourceFile);

      if (!disopt) {
        Optimizer::optimize(llvmModule.get());
      }

      if(emitHrAsm) {
        CodeGenerator::emitHumanReadableAssembly(llvmModule.get());
      } else {
        CodeGenerator::emit(llvmModule.get());
      }

    } catch (SemanticError& e) {
      LOG_S(ERROR) << "tipc: " << e.what();
      LOG_S(ERROR) << "tipc: semantic error";
      exit (EXIT_FAILURE);
    }

  } catch (ParseError& e) {
    LOG_S(ERROR) << "tipc: " << e.what();
    LOG_S(ERROR) << "tipc: parse error";
    exit (EXIT_FAILURE);
  }

}
