#pragma once

#include "ASTVisitor.h"
#include <map>

/*! \class LocalNameCollector
 *  \brief Records local names declared in each function and checks for errors.
 *
 * The local name pass visits a restricted set of nodes:
 * \sa Function to create the instance of the local map and make it current.
 * \sa DeclNode to install declared names in the current local map and
 * to check that a name is declared at most once.
 * \sa VariableExpr to ensure that the referenced name is in the map.
 * Errors are reported by throwing SemanticError exceptions.
 * \sa SemanticError
 */
class LocalNameCollector : public ASTVisitor {
  std::map<std::string, ASTDeclNode*> curMap;
  std::map<std::string, ASTDeclNode*> fMap;
  std::string funName;
  bool first = true;
public:
  LocalNameCollector(std::map<std::string, ASTDeclNode*> fMap) : fMap(fMap) {}

  // this map is public so that the static method can access it
  std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> lMap;

  static std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> build(
      ASTProgram* p, std::map<std::string, ASTDeclNode*> fMap);

  virtual bool visit(ASTFunction * element) override;
  virtual void endVisit(ASTFunction * element) override;
  virtual void endVisit(ASTDeclNode * element) override;
  virtual void endVisit(ASTVariableExpr * element) override;
};

