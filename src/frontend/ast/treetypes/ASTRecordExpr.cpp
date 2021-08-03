#include "ASTRecordExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTRecordExpr::ASTRecordExpr(std::vector<std::unique_ptr<ASTFieldExpr>> FIELDS) {
  for(auto &field : FIELDS) {
    std::shared_ptr<ASTFieldExpr> f = std::move(field);
    this->FIELDS.push_back(f);
  }
}

std::vector<ASTFieldExpr*> ASTRecordExpr::getFields() const {
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

std::ostream& ASTRecordExpr::print(std::ostream &out) const {
  out << "{";
  bool skip = true;
  for (auto &f : getFields()) {
    if (skip) {
      skip = false;
      out << *f;
      continue;
    } 
    out << "," << *f;
  } 
  out << "}";
  return out;
}  // LCOV_EXCL_LINE


std::vector<std::shared_ptr<ASTNode>> ASTRecordExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  for(auto &field : FIELDS) {
    children.push_back(field);
  }
  return children;
}
