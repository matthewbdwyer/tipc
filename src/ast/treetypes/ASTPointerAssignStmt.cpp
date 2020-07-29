#include "ASTPointerAssignStmt.h"
#include "ASTVisitor.h"

void ASTPointerAssignStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLHS()->accept(visitor);
    getRHS()->accept(visitor);
  }
  visitor->endVisit(this);
}
