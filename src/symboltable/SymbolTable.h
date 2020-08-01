#pragma once

#include "ASTVisitor.h"

#include <optional>
#include <map>
#include <vector>

/*
 * The symbol table maps names of identifiers to declaration nodes.
 * There is a global map of for function names and a local map for
 * each function.  In addition it records the set of field names used
 * in the program.
 */ 
class SymbolTable {
  std::map<std::string, ASTDeclNode*> functionNames;
  std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> localNames;
  std::vector<std::string> fieldNames;
public:
  SymbolTable(std::map<std::string, ASTDeclNode*> fMap,
              std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> lMap,
              std::vector<std::string> fSet)
      : functionNames(fMap), localNames(lMap), fieldNames(fSet) {}
  ASTDeclNode* getFunction(std::string s);
  ASTDeclNode* getLocal(std::string s, ASTDeclNode* f);
  std::vector<std::string> getFields();

   /*
   * If SymbolTable is built without error then a unique pointer to it is 
   * found in the value() of the returned result, oherwise 
   * std::nullopt is returned.
   */
  static std::optional<std::unique_ptr<SymbolTable>> build(ASTProgram* p, std::ostream &s);

  static void print(SymbolTable* st, std::ostream &s);
};

