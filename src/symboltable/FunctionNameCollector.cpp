#include "FunctionNameCollector.h"

#include <iostream>

std::map<std::string, ASTDeclNode*> FunctionNameCollector::build(ASTProgram* p, std::ostream &s) {
  FunctionNameCollector visitor(s);
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
    s << "Symbol Error: function name " + decl->getName() + " already declared\n";
  }
  return false;
}
