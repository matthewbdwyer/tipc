#pragma once

#include "ASTExpr.h"

/*! \brief Class for alloc expression
 */
class ASTAllocExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> INIT;
public:
  ASTAllocExpr(std::unique_ptr<ASTExpr> INIT) : INIT(std::move(INIT)) {}
  ASTExpr* getInitializer() const { return INIT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
