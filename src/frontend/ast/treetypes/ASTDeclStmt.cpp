#include "ASTDeclStmt.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTDeclNode*> ASTDeclStmt::getVars() const {
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

std::ostream& ASTDeclStmt::print(std::ostream &out) const {
  out << "var ";
  bool skip = true;
  for (auto &id : getVars()) {
    if (skip) {
      skip = false;
      out << *id;
      continue;
    } 
    out << ", " << *id;
  } 
  out << ";";
  return out;
}  // LCOV_EXCL_LINE
