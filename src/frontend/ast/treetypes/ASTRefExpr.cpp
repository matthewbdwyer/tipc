#include "ASTRefExpr.h"
#include "ASTVisitor.h"

void ASTRefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getVar()->accept(visitor);
  }
  visitor->endVisit(this);
}
