#include "SymbolTable.h"

#include <iostream>

bool symTableBuildError = false;

std::optional<std::unique_ptr<SymbolTable>> 
      SymbolTable::build(AST::Program* p, std::ostream &s) {
  symTableBuildError = false;
  auto fMap = FunctionNameBuilder::build(p, s);
  auto lMap = LocalNameBuilder::build(p, fMap, s);
  return symTableBuildError ? std::nullopt:
         std::make_optional<std::unique_ptr<SymbolTable>>(
             std::make_unique<SymbolTable>(fMap, lMap));
}

AST::DeclNode* SymbolTable::getFunction(std::string s) {
  return functionNames.find(s)->second;
}

AST::DeclNode* SymbolTable::getLocal(std::string s, AST::DeclNode* f) {
  auto lMap = localNames.find(f)->second;
  return lMap.find(s)->second;
}

void SymbolTable::print(SymbolTable* st, std::ostream &s) {
  s << "Functions : {"; 
  auto skip = true;
  for (auto e : st->functionNames) {
    if (skip) {
      skip = false;
      s << e.first;
      continue;
    }
    s << ", " + e.first; 
  }
  s << "}\n";

  for (auto f : st->localNames) {
    s << "Locals for function " + f.first->getName() + " : {";
    skip = true;
    for (auto l : f.second) {
      if (skip) {
        skip = false;
        s << l.first;
        continue;
      }
      s << ", " + l.first; 
    }
    s << "}\n";
  }
}

std::map<std::string, AST::DeclNode*> FunctionNameBuilder::build(AST::Program* p, std::ostream &s) {
  FunctionNameBuilder visitor(s);
  p->accept(&visitor);
  symTableBuildError |= visitor.buildError;
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
    buildError = true;
    s << "Symbol Error: function name " + decl->getName() + " already declared\n";
  }
  return false;
}

std::map<AST::DeclNode*, std::map<std::string, AST::DeclNode*>> LocalNameBuilder::build(
    AST::Program* p, std::map<std::string, AST::DeclNode*> fMap, std::ostream &s) {
  LocalNameBuilder visitor(fMap, s);
  p->accept(&visitor);
  symTableBuildError |= visitor.buildError;
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
        buildError = true;
        s << "Symbol Error: local name " + element->getName() + " redeclared within function\n";
      }
    } else {
      buildError = true;
      s << "Symbol Error: local name " + element->getName() + " already declared as function\n";
    }
  }
}

void LocalNameBuilder::endVisit(AST::VariableExpr * element) {
  if (fMap.count(element->getName()) == 0) {
    if (curMap.count(element->getName()) == 0) {
      buildError = true;
      s << "Symbol Error: variable " + element->getName() + " is undeclared\n";
    }
  }
}

