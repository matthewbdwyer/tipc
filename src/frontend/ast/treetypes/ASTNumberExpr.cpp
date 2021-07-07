#include "ASTNumberExpr.h"
#include "ASTVisitor.h"

#include <iostream>

void ASTNumberExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

std::ostream& ASTNumberExpr::print(std::ostream &out) const {
  out << getValue();
  return out;
}
