#pragma once

#include "ASTExpr.h"

// ASTDeRefExpr - class for dereferencing a pointer expression
class ASTDeRefExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> PTR;
public:
  ASTDeRefExpr(std::unique_ptr<ASTExpr> PTR) : PTR(std::move(PTR)) {}
  ASTExpr* getPtr() { return PTR.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
