#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for if-then-else.
 */
class ASTIfStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> COND;
  std::unique_ptr<ASTStmt> THEN, ELSE;
public:
  ASTIfStmt(std::unique_ptr<ASTExpr> COND, std::unique_ptr<ASTStmt> THEN,
            std::unique_ptr<ASTStmt> ELSE)
      : COND(std::move(COND)), THEN(std::move(THEN)), ELSE(std::move(ELSE)) {}
  ASTExpr* getCondition() const { return COND.get(); }
  ASTStmt* getThen() const { return THEN.get(); }

  /*! \fn getElse
   * \return Else statement if it exists and nullptr otherwise.
   */
  ASTStmt* getElse() const { return ELSE.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
