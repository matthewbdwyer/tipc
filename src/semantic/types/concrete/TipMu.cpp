#include "TipMu.h"
#include "TipTypeVisitor.h"

#include <iostream>

TipMu::TipMu(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t): v(std::move(v)), t(std::move(t)) { }

const std::shared_ptr<TipVar> &TipMu::getV() const {
    return v;
}

const std::shared_ptr<TipType> &TipMu::getT() const {
    return t;
}

bool TipMu::operator==(const TipType &other) const {
    auto mu = dynamic_cast<const TipMu *>(&other);
    if(!mu) {
      return false;
    }
    return *v == *(mu->v) && *t == *(mu->t);
}

bool TipMu::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipMu::print(std::ostream &out) const {
    out << "\u03bc" << *v << "." << *t;
    return out;
}

void TipMu::accept(TipTypeVisitor * visitor) {
  if (visitor->visit(this)) {
    v->accept(visitor);
    t->accept(visitor);
  }
  visitor->endVisit(this);
}
