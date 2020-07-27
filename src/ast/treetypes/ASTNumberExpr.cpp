#include "ASTNumberExpr.h"
#include "ASTVisitor.h"

void ASTNumberExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}
