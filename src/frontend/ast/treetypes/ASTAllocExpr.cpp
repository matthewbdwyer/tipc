#include "ASTAllocExpr.h"
#include "ASTVisitor.h"

void ASTAllocExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
  }
  visitor->endVisit(this);
}
