#pragma once

#include "ASTStmt.h"

/*! \brief Class for block of statements
 */
class ASTBlockStmt : public ASTStmt {
  std::vector<std::unique_ptr<ASTStmt>> STMTS;
public:
  ASTBlockStmt(std::vector<std::unique_ptr<ASTStmt>> STMTS)
      : STMTS(std::move(STMTS)) {}
  std::vector<ASTStmt*> getStmts() const;
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
