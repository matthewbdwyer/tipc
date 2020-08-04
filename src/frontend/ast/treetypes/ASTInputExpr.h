#pragma once

#include "ASTExpr.h"

// InputExpr - class for input expression
class ASTInputExpr : public ASTExpr {
public:
  ASTInputExpr() {}
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
