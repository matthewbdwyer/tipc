#include "CodeGenerator.h"
#include <llvm/IR/Verifier.h>

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"

using namespace llvm;

std::shared_ptr<Module>
CodeGenerator::generate(ASTProgram *program, SemanticAnalysis *analysisResults,
                        std::string fileName) {
  return std::move(program->codegen(analysisResults, fileName));
} // LCOV_EXCL_LINE

void CodeGenerator::emit(llvm::Module *m, std::string filename) {
  if (filename.empty()) {
    filename = m->getModuleIdentifier() + LLVM_BC_EXT;
  }

  std::error_code ec;
  ToolOutputFile result(filename, ec, sys::fs::OF_None);

  // Only enable this routine if the build type is Debug, TIPC_DEBUG definition is declared in src/codegen/CMakeLists.txt
#ifdef TIPC_DEBUG
  //---
  // llvm/IR/Verifier gives more precise errors in codegen than lld link-time
  // errors
  /// Check a module for errors.
  ///
  /// If there are no errors, the function returns false. If an error is
  /// found, a message describing the error is written to OS (if
  /// non-null) and true is returned. -- llvm docs.
  std::string errorMessage;
  llvm::raw_string_ostream errorStream(errorMessage);

  if (llvm::verifyModule(*m, &errorStream)) {
    // Verification failed, print the error message
    llvm::errs() << "ERROR LOG FOR - " + m->getName().str() + "\n========\n";
    errorStream.flush();
    llvm::errs() << errorMessage << "\n========\n";
  }

  //---

#endif

  WriteBitcodeToFile(*m, result.os());
  result.keep();
}

void CodeGenerator::emitHumanReadableAssembly(llvm::Module *m,
                                              std::string filename) {
  if (filename.empty()) {
    filename = m->getModuleIdentifier() + LLVM_ASM_EXT;
  }

  std::error_code ec;
  ToolOutputFile result(filename, ec, sys::fs::OF_None);
  m->print(result.os(), nullptr);
  result.keep();
}
