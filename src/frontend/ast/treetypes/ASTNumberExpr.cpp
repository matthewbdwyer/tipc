#include "ASTNumberExpr.h"
#include "ASTVisitor.h"
#include <iostream>

void ASTNumberExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

std::ostream& ASTNumberExpr::print(std::ostream &out) const {
  std::cout << "ASTNumberExpr::print = " << getValue();
  out << getValue();
  return out;
}
