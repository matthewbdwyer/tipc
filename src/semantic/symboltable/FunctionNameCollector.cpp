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
  auto [_, success] = fMap.try_emplace(decl->getName(), decl);
  if (!success) { // insertion is unsuccessful -> name already declared -> throws error
    throw SemanticError("Symbol error on line " + std::to_string(decl->getLine()) + ": function name " + decl->getName() + " already declared\n");
  }
  return false;
}
