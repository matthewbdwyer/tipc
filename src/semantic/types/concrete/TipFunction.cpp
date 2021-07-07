#include "TipFunction.h"
#include "TipTypeVisitor.h"

#include <sstream>

TipFunction::TipFunction(std::vector<std::shared_ptr<TipType>> params, std::shared_ptr<TipType> ret):
  TipCons(std::move(combine(params, ret))) { }

std::vector<std::shared_ptr<TipType>> TipFunction::combine(
        std::vector<std::shared_ptr<TipType>> params,
        std::shared_ptr<TipType> ret) {
    params.push_back(std::move(ret));
    return params;
}

std::vector<std::shared_ptr<TipType>> TipFunction::getParams() const {
    std::vector<std::shared_ptr<TipType>> params(arguments.begin(), arguments.end()-1);
    return params;
}

std::shared_ptr<TipType> TipFunction::getReturnValue() const {
    return arguments.back();
}

std::ostream &TipFunction::print(std::ostream &out) const {
    out << "(";
    int end_of_args = arguments.size() - 1;
    for(int i = 0; i < end_of_args; i++) {
        out << *arguments.at(i) << (i == end_of_args - 1 ? "" : ",");
    }
    out << ") -> " << *arguments.back();
    return out;
}

bool TipFunction::operator==(const TipType &other) const {
    auto otherTipFunction = dynamic_cast<const TipFunction *>(&other);
    if(!otherTipFunction) {
        return false;
    }

    if(arguments.size() != otherTipFunction->arguments.size()) {
        return false;
    }

    for(int i = 0; i < arguments.size(); i++) {
        if(*(arguments.at(i)) != *(otherTipFunction->arguments.at(i))) {
            return false;
        }
    }

    return *arguments.back() == *(otherTipFunction->arguments.back());
}

bool TipFunction::operator!=(const TipType &other) const {
    return !(*this == other);
}

void TipFunction::accept(TipTypeVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto a : arguments) {
       a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
