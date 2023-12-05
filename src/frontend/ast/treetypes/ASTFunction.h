#pragma once

#include "ASTDeclNode.h"
#include "ASTDeclStmt.h"
#include "ASTNode.h"
#include "ASTStmt.h"

/*! \brief Class for defining the signature, local declarations, and a body of a
 * function.
 */
class ASTFunction : public ASTNode {
  std::shared_ptr<ASTDeclNode> DECL;
  std::vector<std::shared_ptr<ASTDeclNode>> FORMALS;
  std::vector<std::shared_ptr<ASTDeclStmt>> DECLS;
  std::vector<std::shared_ptr<ASTStmt>> BODY;
  bool ISPOLY;

public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTFunction(std::shared_ptr<ASTDeclNode> DECL,
              std::vector<std::shared_ptr<ASTDeclNode>> FORMALS,
              const std::vector<std::shared_ptr<ASTDeclStmt>> &DECLS,
              std::vector<std::shared_ptr<ASTStmt>> BODY, bool ISPOLY)
      : DECL(DECL), FORMALS(FORMALS), DECLS(DECLS), BODY(BODY), ISPOLY(ISPOLY) {
  }
  ~ASTFunction() = default;
  ASTDeclNode *getDecl() const { return DECL.get(); };
  std::string getName() const { return DECL->getName(); };
  bool isPoly() const { return ISPOLY; };
  std::vector<ASTDeclNode *> getFormals() const;
  std::vector<ASTDeclStmt *> getDeclarations() const;
  std::vector<ASTStmt *> getStmts() const;
  void accept(ASTVisitor *visitor) override;
  llvm::Value *codegen() override;

protected:
  std::ostream &print(std::ostream &out) const override;
};
