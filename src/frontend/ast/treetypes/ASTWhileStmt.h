#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for a while loop.
 */
class ASTWhileStmt : public ASTStmt {
  std::shared_ptr<ASTExpr> COND;
  std::shared_ptr<ASTStmt> BODY;
public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTWhileStmt(std::unique_ptr<ASTExpr> COND, std::unique_ptr<ASTStmt> BODY)
      : COND(std::move(COND)), BODY(std::move(BODY)) {}
  ASTExpr* getCondition() const { return COND.get(); }
  ASTStmt* getBody() const { return BODY.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
