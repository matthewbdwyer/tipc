#include "ASTBinaryExpr.h"
#include "ASTVisitor.h"

void ASTBinaryExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTBinaryExpr::print(std::ostream &out) const {
  out << "(" << *getLeft() << getOp() << *getRight() << ")";
  return out;
}  // LCOV_EXCL_LINE

std::vector<std::shared_ptr<ASTNode>> ASTBinaryExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(LEFT);
  children.push_back(RIGHT);
  return children;
}
