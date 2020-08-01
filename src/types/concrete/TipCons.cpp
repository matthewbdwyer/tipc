#include "TipCons.h"

int TipCons::arity() const {
    return arguments.size();
}

bool TipCons::doMatch(TipType * t) const {
    if(TipCons * c = dynamic_cast<TipCons *>(t)) {
        return c->arity() == arity();
    }
    return false;
}

TipCons::TipCons(std::vector<std::shared_ptr<TipType>> arguments) : arguments(std::move(arguments)) { }

const std::vector<std::shared_ptr<TipType>> &TipCons::getArguments() const {
    return arguments;
}
