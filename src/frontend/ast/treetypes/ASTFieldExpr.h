#pragma once

#include "ASTExpr.h"

/*! \brief Class for the field of a record
 */
class ASTFieldExpr : public ASTExpr {
  std::string FIELD;
  std::unique_ptr<ASTExpr> INIT;
public:
  ASTFieldExpr(const std::string &FIELD, std::unique_ptr<ASTExpr> INIT)
      : FIELD(FIELD), INIT(std::move(INIT)) {}
  std::string getField() const { return FIELD; }
  ASTExpr* getInitializer() const { return INIT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
