#pragma once

#include "ASTStmt.h"

/*! \brief Class for block of statements
 */
class ASTBlockStmt : public ASTStmt {
  std::vector<std::shared_ptr<ASTStmt>> STMTS;
public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTBlockStmt(std::vector<std::unique_ptr<ASTStmt>> STMTS);
  std::vector<ASTStmt*> getStmts() const;
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
