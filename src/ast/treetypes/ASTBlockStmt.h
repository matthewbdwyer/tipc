#pragma once

#include "ASTStmt.h"

// ASTBlockStmt - class for block of statements
class ASTBlockStmt : public ASTStmt {
  std::vector<std::unique_ptr<ASTStmt>> STMTS;
public:
  ASTBlockStmt(std::vector<std::unique_ptr<ASTStmt>> STMTS)
      : STMTS(std::move(STMTS)) {}
  std::vector<ASTStmt*> getStmts();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
