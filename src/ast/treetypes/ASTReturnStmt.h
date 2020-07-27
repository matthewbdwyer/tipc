#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

// ASTReturnStmt - class for a return statement
class ASTReturnStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> ARG;
public:
  ASTReturnStmt(std::unique_ptr<ASTExpr> ARG) : ARG(std::move(ARG)) {}
  ASTExpr* getArg() { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
