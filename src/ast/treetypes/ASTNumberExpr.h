#pragma once

#include "ASTExpr.h"

// ASTNumberExpr - expression class for numeric literals
class ASTNumberExpr : public ASTExpr {
  int VAL;
public:
  ASTNumberExpr(int VAL) : VAL(VAL) {}
  int getValue() { return VAL; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
