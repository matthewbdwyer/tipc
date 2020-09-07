#pragma once

#include "ASTExpr.h"

/*! \brief Class for referencing the address of an expression.
 */
class ASTRefExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> VAR;
public:
  ASTRefExpr(std::unique_ptr<ASTExpr> VAR) : VAR(std::move(VAR)) {}
  ASTExpr* getVar() const { return VAR.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
