#include "TipMu.h"
#include "assert.h"
#include <iostream>

TipMu::TipMu(std::shared_ptr<Var> v, std::shared_ptr<Term> t): v(v), t(t) {

}

std::shared_ptr<Term> TipMu::substitute(std::shared_ptr<Term> var, std::shared_ptr<Term> term) {
    // TODO: Not yet implemented..
    if(Var * sv = dynamic_cast<Var *>(v.get())) {
        if(sv == this->v.get()) {
            //return this
        } else {
            //return new TipMu(this->v, this->t->substitute(sv, term));
        }
    }
    assert(0);
}

std::string TipMu::toString() {
    return "";
}

bool TipMu::operator==(const Term &other) const {
    if(auto mu = dynamic_cast<const TipMu *>(&other)) {
        return *v == *(mu->v) && *t == *(mu->t);
    }
    return false;
}

bool TipMu::operator!=(const Term &other) const {
    return !(*this == other);
}
