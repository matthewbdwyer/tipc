#include "SemanticAnalysis.h"
#include "SymbolTable.h"
#include "CheckAssignable.h"
#include "TypeInference.h"
#include "loguru.hpp"

std::unique_ptr<SemanticAnalysis> SemanticAnalysis::analyze(ASTProgram* ast) {
  auto symTable = SymbolTable::build(ast);
  CheckAssignable::check(ast);

  //start: call graph generation
  auto callGraph = CallGraphBuilder::build(ast, symTable.get());
  //end: call graph generation

  auto typeResults = TypeInference::check(ast, symTable.get());
  return std::make_unique<SemanticAnalysis>(std::move(symTable), std::move(typeResults));
}

SymbolTable* SemanticAnalysis::getSymbolTable() {
  return symTable.get();
}; 

TypeInference* SemanticAnalysis::getTypeResults() {
  return typeResults.get();
}; 

CallGraph* SemanticAnalysis::getCallGraph() {
  return callGraph.get();
};