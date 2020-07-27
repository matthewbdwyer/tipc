#include "ASTRecordExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTFieldExpr*> ASTRecordExpr::getFields() {
  return rawRefs(FIELDS);
}

void ASTRecordExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto f : getFields()) {
      f->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
