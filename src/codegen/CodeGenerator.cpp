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
  ToolOutputFile result(m->getModuleIdentifier() + ".bc", ec, sys::fs::F_None);
  WriteBitcodeToFile(*m, result.os());
  result.keep();
}
