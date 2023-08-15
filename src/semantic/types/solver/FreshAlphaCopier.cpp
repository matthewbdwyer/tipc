#include "FreshAlphaCopier.h"

/*
 * The Copier inherits all of the methods above from Substituter, but
 * it overrides the behavior for TipVar and TipAlpha.
 */
std::shared_ptr<TipType> FreshAlphaCopier::copy(TipType *t, ASTNode *c) {
  FreshAlphaCopier visitor;
  visitor.context = c;
  t->accept(&visitor);
  return visitor.getResult();
}

void FreshAlphaCopier::endVisit(TipAlpha *element) {
  visitedTypes.push_back(std::make_shared<TipAlpha>(element->getNode(), context,
                                                    element->getName()));
}
