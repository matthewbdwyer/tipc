#include "ASTVariableExpr.h"
#include "ASTVisitor.h"

void ASTVariableExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

std::ostream& ASTVariableExpr::print(std::ostream &out) const {
  out << getName();
  return out;
}  // LCOV_EXCL_LINE
