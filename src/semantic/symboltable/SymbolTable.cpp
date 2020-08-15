#include "SymbolTable.h"
#include "FunctionNameCollector.h"
#include "LocalNameCollector.h"
#include "FieldNameCollector.h"

#include <sstream>

std::unique_ptr<SymbolTable> SymbolTable::build(ASTProgram* p) {
  auto fMap = FunctionNameCollector::build(p);
  auto lMap = LocalNameCollector::build(p, fMap);
  auto fSet = FieldNameCollector::build(p); 
  return std::make_unique<SymbolTable>(fMap, lMap, fSet);
}

ASTDeclNode* SymbolTable::getFunction(std::string s) {
  auto func = functionNames.find(s);
  if(func == functionNames.end()) {
    return nullptr;
  }
  return func->second;
}

std::vector<ASTDeclNode*> SymbolTable::getFunctions() {
  std::vector<ASTDeclNode*> funDecls;
  for (auto &pair : functionNames) {
    funDecls.push_back(pair.second); 
  }
  return funDecls;
}

ASTDeclNode* SymbolTable::getLocal(std::string s, ASTDeclNode* f) {
  auto lMap = localNames.find(f)->second;
  auto local = lMap.find(s);
  if(local == lMap.end()) {
    return nullptr;
  }
  return local->second;
}

std::vector<ASTDeclNode*> SymbolTable::getLocals(ASTDeclNode* f) {
  auto lMap = localNames.find(f)->second;
  std::vector<ASTDeclNode*> localDecls;
  for (auto &pair : lMap) {
    localDecls.push_back(pair.second); 
  }
  return localDecls;
}


std::vector<std::string> SymbolTable::getFields() {
  return fieldNames;
}

void SymbolTable::print(std::ostream &s) {
  s << "Functions : {"; 
  auto skip = true;
  for (auto e : functionNames) {
    if (skip) {
      skip = false;
      s << e.first;
      continue;
    }
    s << ", " + e.first; 
  }
  s << "}\n";

  s << "Fields : {"; 
  skip = true;
  for (auto n : fieldNames) {
    if (skip) {
      skip = false;
      s << n;
      continue;
    }
    s << ", " + n;
  }
  s << "}\n";

  for (auto f : localNames) {
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
