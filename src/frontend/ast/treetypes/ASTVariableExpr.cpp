#include "ASTVariableExpr.h"
#include "ASTVisitor.h"

void ASTVariableExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}
