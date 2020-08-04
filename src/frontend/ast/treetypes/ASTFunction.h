#pragma once

#include "ASTNode.h"
#include "ASTDeclNode.h"
#include "ASTStmt.h"
#include "ASTDeclStmt.h"

// ASTFunction - signature, local declarations, and a body
class ASTFunction : public ASTNode {
  std::unique_ptr<ASTDeclNode> DECL;
  std::vector<std::unique_ptr<ASTDeclNode>> FORMALS;
  std::vector<std::unique_ptr<ASTDeclStmt>> DECLS;
  std::vector<std::unique_ptr<ASTStmt>> BODY;
public:
  ASTFunction(std::unique_ptr<ASTDeclNode> DECL, 
           std::vector<std::unique_ptr<ASTDeclNode>> FORMALS,
           std::vector<std::unique_ptr<ASTDeclStmt>> DECLS,
           std::vector<std::unique_ptr<ASTStmt>> BODY)
      : DECL(std::move(DECL)), FORMALS(std::move(FORMALS)), 
        DECLS(std::move(DECLS)), BODY(std::move(BODY)) {}
  ASTDeclNode* getDecl() { return DECL.get(); };
  std::string getName() { return DECL->getName(); };
  std::vector<ASTDeclNode*> getFormals();
  std::vector<ASTDeclStmt*> getDeclarations();
  std::vector<ASTStmt*> getStmts();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
