#include "CodeGenerator.h"

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"

using namespace llvm;

std::unique_ptr<Module> CodeGenerator::generate(ASTProgram* program, 
                                SemanticAnalysis* analysisResults, std::string fileName) {
  return std::move(program->codegen(analysisResults, fileName));
}  // LCOV_EXCL_LINE

void CodeGenerator::emit(llvm::Module* m, std::string filename) {
  if(filename.empty())  {
    filename = m->getModuleIdentifier() + LLVM_BC_EXT;
  }

  std::error_code ec;
  ToolOutputFile result(filename, ec, sys::fs::OF_None);
  WriteBitcodeToFile(*m, result.os());
  result.keep();
}

void CodeGenerator::emitHumanReadableAssembly(llvm::Module* m, std::string filename) {
  if(filename.empty())  {
    filename = m->getModuleIdentifier() + LLVM_ASM_EXT;
  }

  std::error_code ec;
  ToolOutputFile result(filename, ec, sys::fs::OF_None);
  m->print(result.os(), nullptr);
  result.keep();
}
