#include "ASTDeclNode.h"
#include "ASTVisitor.h"

void ASTDeclNode::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

std::ostream& ASTDeclNode::print(std::ostream &out) const {
  out << getName();
  return out;
}
