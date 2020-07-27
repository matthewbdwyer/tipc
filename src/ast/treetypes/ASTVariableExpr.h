#pragma once

#include "ASTExpr.h"

// ASTVariableExpr - class for referencing a variable
class ASTVariableExpr : public ASTExpr {
  std::string NAME;
public:
  ASTVariableExpr(std::string NAME) : NAME(NAME) {}
  std::string getName() { return NAME; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
