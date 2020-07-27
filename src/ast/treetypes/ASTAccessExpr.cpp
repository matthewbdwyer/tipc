#include "ASTAccessExpr.h"
#include "ASTVisitor.h"

void ASTAccessExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getRecord()->accept(visitor);
  }
  visitor->endVisit(this);
}
