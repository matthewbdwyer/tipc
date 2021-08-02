#include "SemanticAnalysis.h"
#include "SymbolTable.h"
#include "CheckAssignable.h"
#include "TypeInference.h"
#include "CGB.h"  //call graph builder header
#include "loguru.hpp"

std::unique_ptr<SemanticAnalysis> SemanticAnalysis::analyze(ASTProgram* ast) {
  auto symTable = SymbolTable::build(ast);

  CheckAssignable::check(ast);

//start: call graph generation
  auto cfa = CFAnalyzer::analyze(ast, symTable.get());
  auto callgraph = CallGraphBuilder::build(ast, cfa);
  CallGraphBuilder::printCallGraph(ast -> getFunctions(), callgraph, std::cout);
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

