#include "ASTDeclNode.h"
#include "ASTVisitor.h"

void ASTDeclNode::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}
