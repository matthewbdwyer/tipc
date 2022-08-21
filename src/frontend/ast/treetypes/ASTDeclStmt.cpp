#include "ASTDeclStmt.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTDeclStmt::ASTDeclStmt(std::vector<std::unique_ptr<ASTDeclNode>> VARS) {
  for(auto &var : VARS) {
    std::shared_ptr<ASTDeclNode> d = std::move(var);
    this->VARS.push_back(d);
  }
}

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

std::vector<std::shared_ptr<ASTNode>> ASTDeclStmt::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  for (auto &var : VARS) {
    children.push_back(var);
  }
  return children;
}
