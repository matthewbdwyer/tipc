#pragma once

#include "ASTExpr.h"
#include "ASTStmt.h"

/*! \brief Class for assignment
 */
class ASTAssignStmt : public ASTStmt {
  std::shared_ptr<ASTExpr> LHS, RHS;

public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTAssignStmt(std::shared_ptr<ASTExpr> LHS, std::shared_ptr<ASTExpr> RHS)
      : LHS(LHS), RHS(RHS) {}
  ASTExpr *getLHS() const { return LHS.get(); }
  ASTExpr *getRHS() const { return RHS.get(); }
  void accept(ASTVisitor *visitor) override;
  llvm::Value *codegen() override;

protected:
  std::ostream &print(std::ostream &out) const override;
};
