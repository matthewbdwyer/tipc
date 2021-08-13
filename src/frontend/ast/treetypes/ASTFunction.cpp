#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

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
