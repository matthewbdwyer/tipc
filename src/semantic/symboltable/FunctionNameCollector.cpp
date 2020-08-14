#include "FunctionNameCollector.h"
#include "SemanticError.h"

std::map<std::string, ASTDeclNode*> FunctionNameCollector::build(ASTProgram* p) {
  FunctionNameCollector visitor;
  p->accept(&visitor);
  return visitor.fMap;
}

/*
 * By returning false, this implements a "shallow" visit of the program by skipping
 * all of the function bodies.
 */
bool FunctionNameCollector::visit(ASTFunction * element) {
  auto decl = element->getDecl();
  // check to see if the name has been declared
  if (fMap.count(decl->getName()) == 0) {
    fMap.insert(std::pair<std::string, ASTDeclNode*>(decl->getName(), decl));
  } else {
    throw SemanticError("Symbol error on line " + std::to_string(decl->getLine()) + ": function name " + decl->getName() + " already declared\n");
  }
  return false;
}
