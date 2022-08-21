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

std::vector<std::shared_ptr<ASTNode>> ASTOutputStmt::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(ARG);
  return children;
}
