#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

// ASTVariableAssignStmt - class for assignment
class ASTVariableAssignStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> LHS, RHS;
public:
  ASTVariableAssignStmt(std::unique_ptr<ASTExpr> LHS, std::unique_ptr<ASTExpr> RHS)
      : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  ASTExpr* getLHS() { return LHS.get(); }
  ASTExpr* getRHS() { return RHS.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
