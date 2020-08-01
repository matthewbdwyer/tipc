#include "LocalNameCollector.h"

#include <iostream>

std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> 
LocalNameCollector::build(ASTProgram* p, 
                          std::map<std::string, ASTDeclNode*> fMap, 
                          std::ostream &s) {
  LocalNameCollector visitor(fMap, s);
  p->accept(&visitor);
  return visitor.lMap;
}

bool LocalNameCollector::visit(ASTFunction * element) {
  curMap.clear();
  first = true;
  return true;
}

void LocalNameCollector::endVisit(ASTFunction * element) {
  auto decl = element->getDecl();
  lMap.insert(std::pair<ASTDeclNode*,
                        std::map<std::string, ASTDeclNode*>>(decl, curMap));
}

void LocalNameCollector::endVisit(ASTDeclNode * element) {
  if (first) {
    // first declaration in a function is the function name which is in the function map
    first = false;
  } else {
    if (fMap.count(element->getName()) == 0) {
      if (curMap.count(element->getName()) == 0) {
        curMap.insert(std::pair<std::string, ASTDeclNode*>(element->getName(), element));
      } else {
        s << "Symbol Error: local name " + element->getName() + " redeclared within function\n";
      }
    } else {
      s << "Symbol Error: local name " + element->getName() + " already declared as function\n";
    }
  }
}

void LocalNameCollector::endVisit(ASTVariableExpr * element) {
  if (fMap.count(element->getName()) == 0) {
    if (curMap.count(element->getName()) == 0) {
      s << "Symbol Error: variable " + element->getName() + " is undeclared\n";
    }
  }
}

