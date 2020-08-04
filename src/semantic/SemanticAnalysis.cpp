#include "SemanticAnalysis.h"
#include "SymbolTable.h"
#include "CheckAssignable.h"

std::unique_ptr<SemanticAnalysis> SemanticAnalysis::analyze(ASTProgram* ast) {
  auto symTable = SymbolTable::build(ast);

  CheckAssignable::check(ast);

  // perform type checking

  return std::make_unique<SemanticAnalysis>(std::move(symTable));
}

SymbolTable* SemanticAnalysis::getSymbolTable() {
  return symTable.get();
}; 

/* 
SymbolTable* SemanticAnalysis::getTypes() {
}
*/
