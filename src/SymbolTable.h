#pragma once

#include "ASTVisitor.h"

#include <optional>

/*
 * The symbol table maps names of identifiers to declaration nodes.
 * There is a global map of for function names and a local map for
 * each function.
 */ 
class SymbolTable {
  std::map<std::string, AST::DeclNode*> functionNames;
  std::map<AST::DeclNode*, std::map<std::string, AST::DeclNode*>> localNames;
public:
  SymbolTable(std::map<std::string, AST::DeclNode*> f,
              std::map<AST::DeclNode*, std::map<std::string, AST::DeclNode*>> l)
      : functionNames(f), localNames(l) {}
  AST::DeclNode* getFunction(std::string s);
  AST::DeclNode* getLocal(std::string s, AST::DeclNode* f);

   /*
   * If SymbolTable is built without error then a unique pointer to it is 
   * found in the value() of the returned result, oherwise 
   * std::nullopt is returned.
   */
  static std::optional<std::unique_ptr<SymbolTable>> build(AST::Program* p, std::ostream &s);

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
  std::map<std::string, AST::DeclNode*> fMap;
  static std::map<std::string, AST::DeclNode*> build(AST::Program* p, std::ostream &s);
  virtual bool visit(AST::Function * element) override;
};

/*
 * The local name pass visits a restricted set of nodes:
 * - Functions to create the instance of the local map and make it current
 * - DeclNode to install declared names in the current local map and
 *   to check that a name is declared at most once
 * - VariableExpr to ensure that the referenced name is in the map
 */
class LocalNameBuilder : public ASTVisitor {
  std::map<std::string, AST::DeclNode*> curMap;
  std::map<std::string, AST::DeclNode*> fMap;
  bool first = true;
  bool buildError = false;
  // stream used to emit error messages
  std::ostream &s;
public:
  LocalNameBuilder(std::map<std::string, AST::DeclNode*> fMap, std::ostream &s) : 
      fMap(fMap), s(s) {}
  // this map is public so that the static method can access it
  std::map<AST::DeclNode*, 
           std::map<std::string, AST::DeclNode*>> lMap;
  static std::map<AST::DeclNode*, std::map<std::string, AST::DeclNode*>> build(
      AST::Program* p, std::map<std::string, AST::DeclNode*> fMap, std::ostream &s);
  virtual bool visit(AST::Function * element) override;
  virtual void endVisit(AST::Function * element) override;
  virtual void endVisit(AST::DeclNode * element) override;
  virtual void endVisit(AST::VariableExpr * element) override;
};

