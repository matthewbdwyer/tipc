#pragma once

#include "ASTExpr.h"

// ASTNullExpr - class for a null expression
class ASTNullExpr : public ASTExpr {
public:
  ASTNullExpr() {}
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
