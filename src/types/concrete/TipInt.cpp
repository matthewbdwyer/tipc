#include "TipInt.h"
#include <string>

TipInt::TipInt() { }

bool TipInt::operator==(const TipType &other) const {
    if(auto t = dynamic_cast<TipInt const *>(&other)) {
        return true;
    }
    return false;
}

bool TipInt::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipInt::print(std::ostream &out) const {
    out << std::string("int");
    return out;
}
