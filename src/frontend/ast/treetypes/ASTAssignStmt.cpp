#include "ASTAssignStmt.h"
#include "ASTVisitor.h"

void ASTAssignStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLHS()->accept(visitor);
    getRHS()->accept(visitor);
  }
  visitor->endVisit(this);
}
