#pragma once

#include "ASTExpr.h"
#include "ASTStmt.h"

/*! \brief Class for a error statement
 */
class ASTErrorStmt : public ASTStmt {
  std::shared_ptr<ASTExpr> ARG;

public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTErrorStmt(std::shared_ptr<ASTExpr> ARG) : ARG(ARG) {}
  ASTExpr *getArg() const { return ARG.get(); }
  void accept(ASTVisitor *visitor) override;
  llvm::Value *codegen() override;

protected:
  std::ostream &print(std::ostream &out) const override;
};
