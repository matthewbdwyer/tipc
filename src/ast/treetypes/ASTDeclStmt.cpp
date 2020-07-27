#include "ASTDeclStmt.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTDeclNode*> ASTDeclStmt::getVars() {
  return rawRefs(VARS);
}

void ASTDeclStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto v : getVars()) {
      v->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
