#pragma once

#include "ASTVisitor.h"

#include <optional>

/*
 * The symbol table maps names of identifiers to declaration nodes.
 * There is a global map of for function names and a local map for
 * each function.
 */ 
class SymbolTable {
  std::map<std::string, ASTDeclNode*> functionNames;
  std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> localNames;
public:
  SymbolTable(std::map<std::string, ASTDeclNode*> f,
              std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> l)
      : functionNames(f), localNames(l) {}
  ASTDeclNode* getFunction(std::string s);
  ASTDeclNode* getLocal(std::string s, ASTDeclNode* f);

   /*
   * If SymbolTable is built without error then a unique pointer to it is 
   * found in the value() of the returned result, oherwise 
   * std::nullopt is returned.
   */
  static std::optional<std::unique_ptr<SymbolTable>> build(ASTProgram* p, std::ostream &s);

  static void print(SymbolTable* st, std::ostream &s);
};

/*
 * The symbol table is built in two passes.
 * The first pass collects function names and the second collects locals.
 */

/*
 * The function name pass visits a restricted set of nodes:
 * - Functions to enter the name in the function map and 
 *   to check that a name is declared at most once
 */
class FunctionNameBuilder : public ASTVisitor {
  // stream used to emit error messages
  std::ostream &s;
  bool buildError = false;
public:
  FunctionNameBuilder(std::ostream &s) : s(s) {}
  // this map is public so that the static method can access it
  std::map<std::string, ASTDeclNode*> fMap;
  static std::map<std::string, ASTDeclNode*> build(ASTProgram* p, std::ostream &s);
  virtual bool visit(ASTFunction * element) override;
};

/*
 * The local name pass visits a restricted set of nodes:
 * - Functions to create the instance of the local map and make it current
 * - DeclNode to install declared names in the current local map and
 *   to check that a name is declared at most once
 * - VariableExpr to ensure that the referenced name is in the map
 */
class LocalNameBuilder : public ASTVisitor {
  std::map<std::string, ASTDeclNode*> curMap;
  std::map<std::string, ASTDeclNode*> fMap;
  bool first = true;
  bool buildError = false;
  // stream used to emit error messages
  std::ostream &s;
public:
  LocalNameBuilder(std::map<std::string, ASTDeclNode*> fMap, std::ostream &s) : 
      fMap(fMap), s(s) {}
  // this map is public so that the static method can access it
  std::map<ASTDeclNode*, 
           std::map<std::string, ASTDeclNode*>> lMap;
  static std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> build(
      ASTProgram* p, std::map<std::string, ASTDeclNode*> fMap, std::ostream &s);
  virtual bool visit(ASTFunction * element) override;
  virtual void endVisit(ASTFunction * element) override;
  virtual void endVisit(ASTDeclNode * element) override;
  virtual void endVisit(ASTVariableExpr * element) override;
};

