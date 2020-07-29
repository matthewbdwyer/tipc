#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

// ASTPointerAssignStmt - class for assignment through pointers.
class ASTPointerAssignStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> LHS, RHS;
public:
  ASTPointerAssignStmt(std::unique_ptr<ASTExpr> LHS, std::unique_ptr<ASTExpr> RHS)
      : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  ASTExpr* getLHS() { return LHS.get(); }
  ASTExpr* getRHS() { return RHS.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
