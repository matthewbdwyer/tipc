#include "TypeVars.h"

std::set<std::shared_ptr<TipVar>> TypeVars::collect(TipType* t) {
  TypeVars visitor;
  t->accept(&visitor);
  return visitor.getVars();
}

void TypeVars::endVisit(TipMu * element) {
  vars.erase(element->getV());
}

void TypeVars::endVisit(TipVar * element) {
  vars.insert(std::make_shared<TipVar>(element->getNode()));
}

void TypeVars::endVisit(TipAlpha * element) {
  vars.insert(std::make_shared<TipAlpha>(element->getNode(), element->getName()));
}
