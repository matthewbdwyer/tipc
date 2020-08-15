#include "ASTOutputStmt.h"
#include "ASTVisitor.h"

void ASTOutputStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTOutputStmt::print(std::ostream &out) const {
  out << "output " << *getArg() << ";";
  return out;
}
