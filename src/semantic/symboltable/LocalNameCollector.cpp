#include "LocalNameCollector.h"
#include "SemanticError.h"
#include "loguru.hpp"

std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> 
LocalNameCollector::build(ASTProgram* p, 
                          std::map<std::string, ASTDeclNode*> fMap) {
  LocalNameCollector visitor(fMap);
  p->accept(&visitor);
  return visitor.lMap;
}

bool LocalNameCollector::visit(ASTFunction * element) {
  curMap.clear();
  funName = element->getName();
  first = true;
  return true;
}

void LocalNameCollector::endVisit(ASTFunction * element) {
  auto decl = element->getDecl();
  LOG_S(1) <<"Adding fun [["<< decl->getName()<<"@"<<decl->getLine()<<":"<<decl->getColumn()<<"]] to symbol table.";
  lMap.emplace(decl, std::move(curMap));
}

void LocalNameCollector::endVisit(ASTDeclNode * element) {
  if (first) {
    // first declaration found in visiting a function is the function name which is in the function map so we skip it
    first = false;
  } else {
    if (fMap.count(element->getName()) == 0) {
      auto[_, success] = curMap.try_emplace(element->getName(), element);
      if (success) {
        LOG_S(1) <<"Adding var [["<< element->getName()<<"@"<<element->getLine()<<":"<<element->getColumn()<<"]] to symbol table.";
      } else {
        throw SemanticError("Symbol error line " + std::to_string(element->getLine()) + " in column " + std::to_string(element->getColumn()) +": " + element->getName() + " redeclared in function " + funName + "\n");
      }
    } else {
      throw SemanticError("Symbol error line " + std::to_string(element->getLine()) + " in column " + std::to_string(element->getColumn()) +": " + element->getName() + " already declared as function\n");
    }
  }
}

void LocalNameCollector::endVisit(ASTVariableExpr * element) {
  if (fMap.count(element->getName()) == 0) {
    if (curMap.count(element->getName()) == 0) {
      throw SemanticError("Symbol error line " + std::to_string(element->getLine()) + " in column " + std::to_string(element->getColumn()) +": " + element->getName() + " undeclared in function " + funName + "\n");
    }
  }
}

