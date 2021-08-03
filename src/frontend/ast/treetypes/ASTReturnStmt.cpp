#include "ASTReturnStmt.h"
#include "ASTVisitor.h"

void ASTReturnStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTReturnStmt::print(std::ostream &out) const {
  out << "return " << *getArg() << ";";
  return out;
}


std::vector<std::shared_ptr<ASTNode>> ASTReturnStmt::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(ARG);
  return children;
}
