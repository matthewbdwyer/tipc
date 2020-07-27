#include "SymbolTable.h"

#include <iostream>

bool symTableBuildError = false;

std::optional<std::unique_ptr<SymbolTable>> 
      SymbolTable::build(ASTProgram* p, std::ostream &s) {
  symTableBuildError = false;
  auto fMap = FunctionNameBuilder::build(p, s);
  auto lMap = LocalNameBuilder::build(p, fMap, s);
  return symTableBuildError ? std::nullopt:
         std::make_optional<std::unique_ptr<SymbolTable>>(
             std::make_unique<SymbolTable>(fMap, lMap));
}

ASTDeclNode* SymbolTable::getFunction(std::string s) {
  auto func = functionNames.find(s);
  if(func == functionNames.end()) {
    return nullptr;
  }
  return func->second;
}

ASTDeclNode* SymbolTable::getLocal(std::string s, ASTDeclNode* f) {
  auto lMap = localNames.find(f)->second;
  auto local = lMap.find(s);
  if(local == lMap.end()) {
    return nullptr;
  }
  return local->second;
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

std::map<std::string, ASTDeclNode*> FunctionNameBuilder::build(ASTProgram* p, std::ostream &s) {
  FunctionNameBuilder visitor(s);
  p->accept(&visitor);
  symTableBuildError |= visitor.buildError;
  return visitor.fMap;
}

/*
 * By returning false, this implements a "shallow" visit of the program by skipping
 * all of the function bodies.
 */
bool FunctionNameBuilder::visit(ASTFunction * element) {
  auto decl = element->getDecl();
  // check to see if the name has been declared
  if (fMap.count(decl->getName()) == 0) {
    fMap.insert(std::pair<std::string, ASTDeclNode*>(decl->getName(), decl));
  } else {
    buildError = true;
    s << "Symbol Error: function name " + decl->getName() + " already declared\n";
  }
  return false;
}

std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> LocalNameBuilder::build(
    ASTProgram* p, std::map<std::string, ASTDeclNode*> fMap, std::ostream &s) {
  LocalNameBuilder visitor(fMap, s);
  p->accept(&visitor);
  symTableBuildError |= visitor.buildError;
  return visitor.lMap;
}

bool LocalNameBuilder::visit(ASTFunction * element) {
  curMap.clear();
  first = true;
  return true;
}

void LocalNameBuilder::endVisit(ASTFunction * element) {
  auto decl = element->getDecl();
  lMap.insert(std::pair<ASTDeclNode*,
                        std::map<std::string, ASTDeclNode*>>(decl, curMap));
}

void LocalNameBuilder::endVisit(ASTDeclNode * element) {
  if (first) {
    // first declaration in a function is the function name which is in the function map
    first = false;
  } else {
    if (fMap.count(element->getName()) == 0) {
      if (curMap.count(element->getName()) == 0) {
        curMap.insert(std::pair<std::string, ASTDeclNode*>(element->getName(), element));
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

void LocalNameBuilder::endVisit(ASTVariableExpr * element) {
  if (fMap.count(element->getName()) == 0) {
    if (curMap.count(element->getName()) == 0) {
      buildError = true;
      s << "Symbol Error: variable " + element->getName() + " is undeclared\n";
    }
  }
}

