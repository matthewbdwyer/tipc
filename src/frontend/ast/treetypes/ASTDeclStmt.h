#pragma once

#include "ASTStmt.h"
#include "ASTDeclNode.h"

/*! \brief Class for local variable declaration statement
 */
class ASTDeclStmt : public ASTStmt {
  std::vector<std::shared_ptr<ASTDeclNode>> VARS;
public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTDeclStmt(std::vector<std::unique_ptr<ASTDeclNode>> VARS);
  std::vector<ASTDeclNode*> getVars() const;
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
