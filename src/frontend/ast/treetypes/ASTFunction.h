#pragma once

#include "ASTNode.h"
#include "ASTDeclNode.h"
#include "ASTStmt.h"
#include "ASTDeclStmt.h"

/*! \brief Class for defining the signature, local declarations, and a body of a function.
 */
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
  ASTDeclNode* getDecl() const { return DECL.get(); };
  std::string getName() const { return DECL->getName(); };
  std::vector<ASTDeclNode*> getFormals() const;
  std::vector<ASTDeclStmt*> getDeclarations() const;
  std::vector<ASTStmt*> getStmts() const;
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
