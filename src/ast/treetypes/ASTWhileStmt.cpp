#include "ASTWhileStmt.h"
#include "ASTVisitor.h"

void ASTWhileStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCondition()->accept(visitor);
    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}
