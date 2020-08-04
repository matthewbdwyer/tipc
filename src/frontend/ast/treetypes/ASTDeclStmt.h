#pragma once

#include "ASTStmt.h"
#include "ASTDeclNode.h"

// ASTDeclStmt - class for local variable declarations
class ASTDeclStmt : public ASTStmt {
  std::vector<std::unique_ptr<ASTDeclNode>> VARS;
public:
  ASTDeclStmt(std::vector<std::unique_ptr<ASTDeclNode>> VARS) 
          : VARS(std::move(VARS)) {}
  std::vector<ASTDeclNode*> getVars();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
