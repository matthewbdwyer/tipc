#include "ASTAccessExpr.h"
#include "ASTVisitor.h"

void ASTAccessExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getRecord()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTAccessExpr::print(std::ostream &out) const {
  out << "(" << *getRecord() << "." << getField() << ")";
  return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTAccessExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(RECORD);
  return children;
}
