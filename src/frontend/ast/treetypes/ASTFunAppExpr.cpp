#include "ASTFunAppExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTFunAppExpr::ASTFunAppExpr(std::shared_ptr<ASTExpr> FUN,
                             std::vector<std::shared_ptr<ASTExpr>> ACTUALS) {
  this->FUN = FUN;

  for (auto &actual : ACTUALS) {
    std::shared_ptr<ASTExpr> a = actual;
    this->ACTUALS.push_back(a);
  }
}

std::vector<ASTExpr *> ASTFunAppExpr::getActuals() const {
  return rawRefs(ACTUALS);
}

void ASTFunAppExpr::accept(ASTVisitor *visitor) {
  if (visitor->visit(this)) {
    getFunction()->accept(visitor);
    for (auto a : getActuals()) {
      a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream &ASTFunAppExpr::print(std::ostream &out) const {
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
} // LCOV_EXCL_LINE

std::vector<std::shared_ptr<ASTNode>> ASTFunAppExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  children.push_back(FUN);
  for (auto &actual : ACTUALS) {
    children.push_back(actual);
  }
  return children;
}
