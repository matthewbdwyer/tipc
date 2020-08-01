#include "TipInt.h"
#include <string>

TipInt::TipInt() { }

bool TipInt::operator==(const TipType &other) const {
    auto otherTipInt = dynamic_cast<TipInt const *>(&other);
    if(!otherTipInt) {
        return false;
    }

    return true;
}

bool TipInt::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipInt::print(std::ostream &out) const {
    out << std::string("int");
    return out;
}
