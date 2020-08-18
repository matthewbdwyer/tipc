#pragma once

#include "ASTExpr.h"

/*! \brief Class for function call expressions
 */
class ASTFunAppExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> FUN;
  std::vector<std::unique_ptr<ASTExpr>> ACTUALS;
public:
  ASTFunAppExpr(std::unique_ptr<ASTExpr> FUN,
                std::vector<std::unique_ptr<ASTExpr>> ACTUALS)
      : FUN(std::move(FUN)), ACTUALS(std::move(ACTUALS)) {}
  ASTExpr* getFunction() const { return FUN.get(); }
  std::vector<ASTExpr*> getActuals() const;
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
