#pragma once

#include "ASTExpr.h"

/*! \brief Class for referencing a variable.
 */
class ASTVariableExpr : public ASTExpr {
  std::string NAME;
public:
  ASTVariableExpr(std::string NAME) : NAME(NAME) {}
  std::string getName() const { return NAME; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
