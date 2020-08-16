#include "TipCons.h"

int TipCons::arity() const {
    return arguments.size();
}

bool TipCons::doMatch(TipType const * t) const {
    auto tipCons = dynamic_cast<TipCons const *>(t);
    if(!tipCons) {
        return false;
    }

    return tipCons->arity() == arity();
}

TipCons::TipCons(std::vector<std::shared_ptr<TipType>> arguments) : arguments(std::move(arguments)) { }

const std::vector<std::shared_ptr<TipType>> &TipCons::getArguments() const {
    return arguments;
}

std::set<std::shared_ptr<TipVar>> TipCons::freeVars() const {
    return std::set<std::shared_ptr<TipVar>>();
}

void TipCons::subst(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t) {

}
