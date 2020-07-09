#pragma once

#include "ASTVisitor.h"

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
  void print();
  static std::unique_ptr<SymbolTable> build(AST::Program* p);
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
public:
  // this map is public so that the static method can access it
  std::map<std::string, AST::DeclNode*> fMap;
  static std::map<std::string, AST::DeclNode*> build(AST::Program* p);
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
public:
  LocalNameBuilder(std::map<std::string, AST::DeclNode*> fMap) : fMap(fMap) {}
  // this map is public so that the static method can access it
  std::map<AST::DeclNode*, 
           std::map<std::string, AST::DeclNode*>> lMap;
  static std::map<AST::DeclNode*, std::map<std::string, AST::DeclNode*>> build(
      AST::Program* p, std::map<std::string, AST::DeclNode*> fMap);
  virtual bool visit(AST::Function * element) override;
  virtual void endVisit(AST::Function * element) override;
  virtual void endVisit(AST::DeclNode * element) override;
  virtual void endVisit(AST::VariableExpr * element) override;
};

