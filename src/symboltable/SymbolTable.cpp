#include "SymbolTable.h"
#include "FunctionNameCollector.h"
#include "LocalNameCollector.h"
#include "FieldNameCollector.h"

#include <iostream>
#include <sstream>

bool symTableBuildError = false;

std::optional<std::unique_ptr<SymbolTable>> 
      SymbolTable::build(ASTProgram* p, std::ostream &s) {
  std::stringstream errorStream;
  auto fMap = FunctionNameCollector::build(p, errorStream);
  auto lMap = LocalNameCollector::build(p, fMap, errorStream);
  auto fSet = FieldNameCollector::build(p); 
  // Errors are indicted by the pressence of messages in the error stream
  if (errorStream.str() == "") {
    return std::make_optional<std::unique_ptr<SymbolTable>>(
                   std::make_unique<SymbolTable>(fMap, lMap, fSet));
  } else {
    // output the error messages
    s << errorStream.str(); 
    return std::nullopt;
  }
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

std::vector<std::string> SymbolTable::getFields() {
  return fieldNames;
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

  s << "Fields : {"; 
  skip = true;
  for (auto n : st->fieldNames) {
    if (skip) {
      skip = false;
      s << n;
      continue;
    }
    s << ", " + n;
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
