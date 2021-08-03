#include "ASTWhileStmt.h"
#include "ASTVisitor.h"

void ASTWhileStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCondition()->accept(visitor);
    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTWhileStmt::print(std::ostream &out) const {
  out << "while (" << *getCondition() << ") " << *getBody();
  return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTWhileStmt::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(COND);
  children.push_back(BODY);
  return children;
}
