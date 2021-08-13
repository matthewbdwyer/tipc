#include "ASTBlockStmt.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTStmt*> ASTBlockStmt::getStmts() const {
  return rawRefs(STMTS);
}

void ASTBlockStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream& ASTBlockStmt::print(std::ostream &out) const {
  out << "{ ";
  for (auto &s : getStmts()) {
    out << *s << " ";
  }
  out << "}";
  return out;
}  // LCOV_EXCL_LINE
