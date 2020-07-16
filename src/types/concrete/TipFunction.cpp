#include "TipFunction.h"
#include <sstream>

TipFunction::TipFunction(std::vector<std::shared_ptr<TipType>> params, std::shared_ptr<TipType> ret):
  params(std::move(params)), ret(std::move(ret)) { };

int TipFunction::arity() {
    // +1 for the return type.
    return arguments.size() + 1;
}

std::ostream &TipFunction::print(std::ostream &out) const {
    out << "(";
    int i = 0;
    for(auto&& param : params) {
        out << *param << (++i == params.size() ? "" : ",");
    }
    out << ") -> " << *ret;
    return out;
}

bool TipFunction::operator==(const TipType &other) const {
    if(auto tipFunction = dynamic_cast<const TipFunction *>(&other)) {
        if(arguments.size() != tipFunction->arguments.size()) {
            return false;
        }

        for(int i = 0; i < arguments.size(); i++) {
            if(*(arguments.at(i)) != *(tipFunction->arguments.at(i))) {
                return false;
            }
        }

        return *ret == *(tipFunction->ret);
    }
    return false;
}

bool TipFunction::operator!=(const TipType &other) const {
    return !(*this == other);
}

