#pragma once

#include "ASTExpr.h"

/*! \brief Class for a binary operator.
 */
class ASTBinaryExpr : public ASTExpr {
  std::string OP;
  std::unique_ptr<ASTExpr> LEFT, RIGHT;
public:
  ASTBinaryExpr(const std::string &OP, std::unique_ptr<ASTExpr> LEFT,
             std::unique_ptr<ASTExpr> RIGHT)
      : OP(OP), LEFT(std::move(LEFT)), RIGHT(std::move(RIGHT)) {}
  std::string getOp() const { return OP; }
  ASTExpr* getLeft() const { return LEFT.get(); }
  ASTExpr* getRight() const { return RIGHT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
