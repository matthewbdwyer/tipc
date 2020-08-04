#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

// ASTOutputStmt - class for a output statement
class ASTOutputStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> ARG;
public:
  ASTOutputStmt(std::unique_ptr<ASTExpr> ARG) : ARG(std::move(ARG)) {}
  ASTExpr* getArg() { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
