#include "ASTNullExpr.h"
#include "ASTVisitor.h"

void ASTNullExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

std::ostream& ASTNullExpr::print(std::ostream &out) const {
  out << "null";
  return out;
}
