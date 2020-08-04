#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

//! \brief Class for if-then-else.
class ASTIfStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> COND;
  std::unique_ptr<ASTStmt> THEN, ELSE;
public:
  ASTIfStmt(std::unique_ptr<ASTExpr> COND, std::unique_ptr<ASTStmt> THEN,
            std::unique_ptr<ASTStmt> ELSE)
      : COND(std::move(COND)), THEN(std::move(THEN)), ELSE(std::move(ELSE)) {}
  ASTExpr* getCondition() { return COND.get(); }
  ASTStmt* getThen() { return THEN.get(); }
  ASTStmt* getElse() { return ELSE.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
