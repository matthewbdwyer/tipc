#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTFunction::ASTFunction(std::unique_ptr<ASTDeclNode> DECL, std::vector<std::unique_ptr<ASTDeclNode>> FORMALS,
                         const std::vector<std::unique_ptr<ASTDeclStmt>> &DECLS,
                         std::vector<std::unique_ptr<ASTStmt>> BODY) {

  this->DECL = std::move(DECL);

  for(auto &formal : FORMALS) {
    std::shared_ptr<ASTDeclNode> f = std::move(formal);
    this->FORMALS.push_back(f);
  }

  for(auto &d : DECLS) {
    this->DECLS.push_back(std::move(const_cast<std::unique_ptr<ASTDeclStmt>&>(d)));
  }

  for(auto &stmt : BODY) {
    std::shared_ptr<ASTStmt> s = std::move(stmt);
    this->BODY.push_back(s);
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
