#include "Copier.h"

/*
 * The Copier inherits all of the methods above from Substituter, but
 * it overrides the behavior for TipVar and TipAlpha.
 */
std::shared_ptr<TipType> Copier::copy(std::shared_ptr<TipType> t) {
  Copier visitor;
  t->accept(&visitor);
  return visitor.getResult();
}

void Copier::endVisit(TipVar *element) {
  visitedTypes.push_back(std::make_shared<TipVar>(element->getNode()));
}

void Copier::endVisit(TipAlpha *element) {
  visitedTypes.push_back(
      std::make_shared<TipAlpha>(element->getNode(), element->getName()));
}
