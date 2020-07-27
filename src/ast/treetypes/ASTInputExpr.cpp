#include "ASTInputExpr.h"
#include "ASTVisitor.h"

void ASTInputExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}
