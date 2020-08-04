#include "TipAlpha.h"
#include <sstream>

TipAlpha::TipAlpha(std::string const av): alphaValue(av) {};

std::ostream &TipAlpha::print(std::ostream &out) const {
    out << "\u03B1<" << alphaValue << ">";
    return out;
}

bool TipAlpha::operator==(const TipType &other) const {
    auto otherTipAlpha = dynamic_cast<const TipAlpha *>(&other);
    if(!otherTipAlpha) {
        return false;
    }

    return alphaValue == otherTipAlpha->getAlphaValue();
}

bool TipAlpha::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::string const & TipAlpha::getAlphaValue() const {
    return alphaValue;
}
