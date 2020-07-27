#include "ASTBlockStmt.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTStmt*> ASTBlockStmt::getStmts() {
  return rawRefs(STMTS);
}

void ASTBlockStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
