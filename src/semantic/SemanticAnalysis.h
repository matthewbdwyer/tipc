#pragma once

#include "ASTProgram.h"
#include "SymbolTable.h"
#include "CheckAssignable.h"
#include <memory>

class SemanticAnalysis {
  std::unique_ptr<SymbolTable> symTable;
  // TBD type results

public:
  SemanticAnalysis(std::unique_ptr<SymbolTable> symTable) : symTable(std::move(symTable)) {}

  static std::unique_ptr<SemanticAnalysis> analyze(ASTProgram* ast);

  SymbolTable* getSymbolTable(); 

  // SymbolTable* getTypes(); 
};
