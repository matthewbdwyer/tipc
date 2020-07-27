#include "TipAlpha.h"
#include <sstream>

TipAlpha::TipAlpha(std::string x): x(x) {};

std::ostream &TipAlpha::print(std::ostream &out) const {
    out << "\u03B1";
    return out;
}

bool TipAlpha::operator==(const TipType &other) const {
    if(auto t = dynamic_cast<const TipAlpha *>(&other)) {
        return x == t->x;
    }
    return false;
}

bool TipAlpha::operator!=(const TipType &other) const {
    return !(*this == other);
}
