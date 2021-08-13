#include "ASTFunAppExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTExpr*> ASTFunAppExpr::getActuals() const {
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

std::ostream& ASTFunAppExpr::print(std::ostream &out) const {
  out << *getFunction() << "(";
  bool skip = true;
  for (auto &arg : getActuals()) {
    if (skip) {
      skip = false;
      out << *arg;
      continue;
    }
    out << ", " << *arg;
  }
  out << ")";
  return out;
}  // LCOV_EXCL_LINE
