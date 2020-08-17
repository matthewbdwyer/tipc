#include "TipCons.h"
#include "TipTypeVisitor.h"

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

void TipCons::setArguments(std::vector<std::shared_ptr<TipType>> &a) {
    arguments = a;
}

const std::vector<std::shared_ptr<TipType>> &TipCons::getArguments() const {
    return arguments;
}
