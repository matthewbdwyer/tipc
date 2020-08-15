#include "TipMu.h"
#include <ostream>

TipMu::TipMu(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t)
        : v(v), t(t) { }

bool TipMu::operator==(const TipType &other) const {
    if(auto mu = dynamic_cast<const TipMu *>(&other)) {
        return *v == *(mu->v) && *t == *(mu->t);
    }
    return false;
}

bool TipMu::operator!=(const Term &other) const {
    return !(*this == other);
}

std::ostream& TipMu::print(std::ostream &out) const {
    out << "\u03bc" << *v << "." << *t;
    return out;
}

