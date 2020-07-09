#include "SymbolTable.h"

#include <iostream>

std::unique_ptr<SymbolTable> SymbolTable::build(AST::Program* p) {
  auto fMap = FunctionNameBuilder::build(p);
  auto lMap = LocalNameBuilder::build(p, fMap);
  return std::make_unique<SymbolTable>(fMap, lMap);
}

AST::DeclNode* SymbolTable::getFunction(std::string s) {
  return functionNames.find(s)->second;
}

AST::DeclNode* SymbolTable::getLocal(std::string s, AST::DeclNode* f) {
  auto lMap = localNames.find(f)->second;
  return lMap.find(s)->second;
}

void SymbolTable::print() {
  std::cout << "Functions : {"; 
  auto skip = true;
  for (auto e : functionNames) {
    if (skip) {
      skip = false;
      std::cout << e.first;
      continue;
    }
    std::cout << ", " + e.first; 
  }
  std::cout << "}\n";

  for (auto f : localNames) {
    std::cout << "Locals for function " + f.first->getName() + " : {";
    skip = true;
    for (auto l : f.second) {
      if (skip) {
        skip = false;
        std::cout << l.first;
        continue;
      }
      std::cout << ", " + l.first; 
    }
    std::cout << "}\n";
  }
}

std::map<std::string, AST::DeclNode*> FunctionNameBuilder::build(AST::Program* p) {
  FunctionNameBuilder visitor;
  p->accept(&visitor);
  return visitor.fMap;
}

/*
 * By returning false, this implements a "shallow" visit of the program by skipping
 * all of the function bodies.
 */
bool FunctionNameBuilder::visit(AST::Function * element) {
  auto decl = element->getDecl();
  // check to see if the name has been declared
  if (fMap.count(decl->getName()) == 0) {
    fMap.insert(std::pair<std::string, AST::DeclNode*>(decl->getName(), decl));
  } else {
    std::cerr << "Symbol Error: function name " + decl->getName() + " already declared\n";
  }
  return false;
}

std::map<AST::DeclNode*, std::map<std::string, AST::DeclNode*>> LocalNameBuilder::build(
    AST::Program* p, std::map<std::string, AST::DeclNode*> fMap) {
  LocalNameBuilder visitor(fMap);
  p->accept(&visitor);
  return visitor.lMap;
}

bool LocalNameBuilder::visit(AST::Function * element) {
  curMap.clear();
  first = true;
  return true;
}

void LocalNameBuilder::endVisit(AST::Function * element) {
  auto decl = element->getDecl();
  lMap.insert(std::pair<AST::DeclNode*,
                        std::map<std::string, AST::DeclNode*>>(decl, curMap));
}

void LocalNameBuilder::endVisit(AST::DeclNode * element) {
  if (first) {
    // first declaration in a function is the function name which is in the function map
    first = false;
  } else {
    if (fMap.count(element->getName()) == 0) {
      if (curMap.count(element->getName()) == 0) {
        curMap.insert(std::pair<std::string, AST::DeclNode*>(element->getName(), element));
      } else {
        std::cerr << "Symbol Error: local name " + element->getName() + " redeclared within function\n";
      }
    } else {
      std::cerr << "Symbol Error: local name " + element->getName() + " already declared as function\n";
    }
  }
}

void LocalNameBuilder::endVisit(AST::VariableExpr * element) {
  if (fMap.count(element->getName()) == 0) {
    if (curMap.count(element->getName()) == 0) {
      std::cerr << "Symbol Error: variable " + element->getName() + " is undeclared\n";
    }
  }
}

