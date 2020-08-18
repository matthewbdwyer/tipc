#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for assignment
 */
class ASTAssignStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> LHS, RHS;
public:
  ASTAssignStmt(std::unique_ptr<ASTExpr> LHS, std::unique_ptr<ASTExpr> RHS)
      : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  ASTExpr* getLHS() const { return LHS.get(); }
  ASTExpr* getRHS() const { return RHS.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
