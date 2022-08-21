#include "ASTFieldExpr.h"
#include "ASTVisitor.h"

void ASTFieldExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTFieldExpr::print(std::ostream &out) const {
  out << getField() << ":" << *getInitializer();
  return out;
}  // LCOV_EXCL_LINE

std::vector<std::shared_ptr<ASTNode>> ASTFieldExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(INIT);
  return children;
}
