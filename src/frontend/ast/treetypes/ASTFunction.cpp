#include "ASTFunction.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTDeclNode*> ASTFunction::getFormals() {
  return rawRefs(FORMALS);
}

std::vector<ASTDeclStmt*> ASTFunction::getDeclarations() {
  return rawRefs(DECLS);
}

std::vector<ASTStmt*> ASTFunction::getStmts() {
  return rawRefs(BODY);
}

void ASTFunction::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getDecl()->accept(visitor);
    for (auto p : getFormals()) {
      p->accept(visitor);
    }
    for (auto d : getDeclarations()) {
      d->accept(visitor);
    }
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
