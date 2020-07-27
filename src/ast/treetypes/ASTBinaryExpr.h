#pragma once

#include "ASTExpr.h"

// ASTBinaryExpr - class for a binary operator.
class ASTBinaryExpr : public ASTExpr {
  std::string OP;
  std::unique_ptr<ASTExpr> LEFT, RIGHT;
public:
  ASTBinaryExpr(const std::string &OP, std::unique_ptr<ASTExpr> LEFT,
             std::unique_ptr<ASTExpr> RIGHT)
      : OP(OP), LEFT(std::move(LEFT)), RIGHT(std::move(RIGHT)) {}
  std::string getOp() { return OP; }
  ASTExpr* getLeft() { return LEFT.get(); }
  ASTExpr* getRight() { return RIGHT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
