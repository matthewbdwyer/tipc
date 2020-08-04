#include "ASTNullExpr.h"
#include "ASTVisitor.h"

void ASTNullExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}
