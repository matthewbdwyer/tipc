#include "SemanticAnalysis.h"
#include "SymbolTable.h"
#include "CheckAssignable.h"
#include "TypeInference.h"

std::unique_ptr<SemanticAnalysis> SemanticAnalysis::analyze(ASTProgram* ast) {
  auto symTable = SymbolTable::build(ast);

  CheckAssignable::check(ast);

  auto typeResults = TypeInference::check(ast, symTable.get());

  return std::make_unique<SemanticAnalysis>(std::move(symTable), std::move(typeResults));
}

SymbolTable* SemanticAnalysis::getSymbolTable() {
  return symTable.get();
}; 

TypeInference* SemanticAnalysis::getTypeResults() {
  return typeResults.get();
}; 

