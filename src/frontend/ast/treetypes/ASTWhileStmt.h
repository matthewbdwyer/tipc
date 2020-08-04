#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

//! \brief Class for a while loop.
class ASTWhileStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> COND;
  std::unique_ptr<ASTStmt> BODY;
public:
  ASTWhileStmt(std::unique_ptr<ASTExpr> COND, std::unique_ptr<ASTStmt> BODY)
      : COND(std::move(COND)), BODY(std::move(BODY)) {}
  ASTExpr* getCondition() { return COND.get(); }
  ASTStmt* getBody() { return BODY.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
