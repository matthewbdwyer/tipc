#include "ASTErrorStmt.h"
#include "ASTVisitor.h"

void ASTErrorStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}
