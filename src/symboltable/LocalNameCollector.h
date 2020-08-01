#pragma once

#include "ASTVisitor.h"

/*
 * The local name pass visits a restricted set of nodes:
 * - Functions to create the instance of the local map and make it current
 * - DeclNode to install declared names in the current local map and
 *   to check that a name is declared at most once
 * - VariableExpr to ensure that the referenced name is in the map
 */
class LocalNameCollector : public ASTVisitor {
  std::map<std::string, ASTDeclNode*> curMap;
  std::map<std::string, ASTDeclNode*> fMap;
  bool first = true;
  // stream used to emit error messages
  std::ostream &s;
public:
  LocalNameCollector(std::map<std::string, ASTDeclNode*> fMap, std::ostream &s) : 
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

