#pragma once

#include "ASTVisitor.h"

/*
 * The function name builder visits a restricted set of nodes:
 * - Functions to enter the name in the function map and 
 *   to check that a name is declared at most once
 */
class FunctionNameCollector : public ASTVisitor {
  // stream used to emit error messages
  std::ostream &s;
  bool buildError = false;
public:
  FunctionNameCollector(std::ostream &s) : s(s) {}
  // this map is public so that the static method can access it
  std::map<std::string, ASTDeclNode*> fMap;
  static std::map<std::string, ASTDeclNode*> build(ASTProgram* p, std::ostream &s);
  virtual bool visit(ASTFunction * element) override;
};

