#include "CodeGenerator.h"

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"

using namespace llvm;

std::unique_ptr<Module> CodeGenerator::generate(ASTProgram* program, 
                                SemanticAnalysis* analysisResults, std::string fileName) {
  return std::move(program->codegen(analysisResults, fileName));
}

void CodeGenerator::emit(Module* m) {
  std::error_code ec;
  ToolOutputFile result(m->getModuleIdentifier() + LLVM_BC_EXT, ec, sys::fs::F_None);
  WriteBitcodeToFile(*m, result.os());
  result.keep();
}

void CodeGenerator::emitHumanReadableAssembly(llvm::Module *m) {
    std::error_code ec;
    ToolOutputFile result(m->getModuleIdentifier() + LLVM_ASM_EXT, ec, sys::fs::F_None);
    m->print(result.os(), nullptr);
    result.keep();
}
