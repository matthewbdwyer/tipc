#include "SemanticAnalysis.h"
#include "SymbolTable.h"
#include "CheckAssignable.h"
#include "TypeInference.h"

std::unique_ptr<SemanticAnalysis> SemanticAnalysis::analyze(ASTProgram* ast) {
  auto symTable = SymbolTable::build(ast);

  CheckAssignable::check(ast);

  TypeInference::inferandcheck(ast, symTable.get());

  return std::make_unique<SemanticAnalysis>(std::move(symTable));
}

SymbolTable* SemanticAnalysis::getSymbolTable() {
  return symTable.get();
}; 

