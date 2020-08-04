#include "ASTFunAppExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTExpr*> ASTFunAppExpr::getActuals() {
  return rawRefs(ACTUALS);
}

void ASTFunAppExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getFunction()->accept(visitor);
    for (auto a : getActuals()) {
      a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
