#pragma once

#include "ASTExpr.h"

/*! \brief Class for dereferencing a pointer expression
 */
class ASTDeRefExpr : public ASTExpr {
  std::shared_ptr<ASTExpr> PTR;
public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTDeRefExpr(std::unique_ptr<ASTExpr> PTR) : PTR(std::move(PTR)) {}
  ASTExpr* getPtr() const { return PTR.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
