#include "ASTDeRefExpr.h"
#include "ASTVisitor.h"

void ASTDeRefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getPtr()->accept(visitor);
  }
  visitor->endVisit(this);
}
