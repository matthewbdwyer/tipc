#pragma once

#include "ASTVisitor.h"
#include <map>

/*! \class FunctionNameCollector
 *  \brief Collects the names of functions declared in the program.
 *
 * The function name builder visits a restricted set of nodes, just functions,
 * It records the name in the function map and checks that a name is declared at most once.
 * Errors are reported by throwing SemanticError exceptions.
 * \sa SemanticError
 */
class FunctionNameCollector : public ASTVisitor {
public:
  FunctionNameCollector() = default;
  // this map is public so that the static method can access it
  std::map<std::string, ASTDeclNode*> fMap;
  static std::map<std::string, ASTDeclNode*> build(ASTProgram* p);
  virtual bool visit(ASTFunction * element) override;
};

