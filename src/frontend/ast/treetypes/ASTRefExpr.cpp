#include "ASTRefExpr.h"
#include "ASTVisitor.h"

void ASTRefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getVar()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTRefExpr::print(std::ostream &out) const {
  out << "&" << *getVar();
  return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTRefExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(VAR);
  return children;
}
