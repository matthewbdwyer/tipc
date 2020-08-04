#include "ASTReturnStmt.h"
#include "ASTVisitor.h"

void ASTReturnStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}
