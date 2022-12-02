#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTFunction::ASTFunction(std::shared_ptr<ASTDeclNode> DECL, 
                         std::vector<std::shared_ptr<ASTDeclNode>> FORMALS,
                         std::vector<std::shared_ptr<ASTDeclStmt>> DECLS,
                         std::vector<std::shared_ptr<ASTStmt>> BODY,
                         std::vector<std::shared_ptr<ASTExpr>> RETURNS) {

  this->DECL = DECL;

  for(auto &formal : FORMALS) {
    this->FORMALS.push_back(formal);
  }

  for(auto &decl : DECLS) {
    this->DECLS.push_back(decl);
  }

  for(auto &stmt : BODY) {
    this->BODY.push_back(stmt);
  }

  for(auto &ret : RETURNS) {
    this->RETURNS.push_back(ret);
  }
}

std::vector<ASTDeclNode*> ASTFunction::getFormals() const {
  return rawRefs(FORMALS);
}

std::vector<ASTDeclStmt*> ASTFunction::getDeclarations() const {
  return rawRefs(DECLS);
}

std::vector<ASTStmt*> ASTFunction::getStmts() const {
  return rawRefs(BODY);
}

namespace {

class CollectReturnExprs :  public ASTVisitor {
  std::vector<std::shared_ptr<ASTExpr>> returnExprs;
public:
  CollectReturnExprs() = default;
  void endVisit(ASTReturnStmt * element) {
    returnExprs.push_back(element->getSharedArg());
  }
  std::vector<std::shared_ptr<ASTExpr>> getExprs() {
    return returnExprs;
  }
};

}

//! \brief Calculate the expressions in the return statements of the function
std::vector<ASTExpr*> ASTFunction::getReturnExprs() {
  if (RETURNS.size() == 0) {
    CollectReturnExprs visitor;
    for (auto s : getStmts()) {
      s->accept(&visitor); 
    }
    for (auto &e : visitor.getExprs()) {
      RETURNS.push_back(e);
    }
  }
  return rawRefs(RETURNS);
}

void ASTFunction::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getDecl()->accept(visitor);
    for (auto p : getFormals()) {
      p->accept(visitor);
    }
    for (auto d : getDeclarations()) {
      d->accept(visitor);
    }
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

//! \brief Print an abbreviated unique string for the function
std::ostream& ASTFunction::print(std::ostream &out) const {
  out << *getDecl() << "(";
  bool skip = true;
  for (auto &p : getFormals()) {
    if (skip) {
      skip = false;
      out << *p;
      continue;
    }
    out << "," << *p;
  }
  out << ") {...}";
  return out;
}  // LCOV_EXCL_LINE

std::vector<std::shared_ptr<ASTNode>> ASTFunction::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;

  children.push_back(DECL);
  for (auto &formal : FORMALS) {
    children.push_back(formal);
  }
  for (auto &decl : DECLS) {
    children.push_back(decl);
  }
  for (auto &stmt : BODY) {
    children.push_back(stmt);
  }

  return children;
}
