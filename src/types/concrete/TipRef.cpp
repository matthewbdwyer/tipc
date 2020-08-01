#include "TipRef.h"
#include <sstream>

TipRef::TipRef(std::shared_ptr<TipType> of)
  : TipCons(std::move(std::vector<std::shared_ptr<TipType>> {of})) { }

bool TipRef::operator==(const TipType &other) const {
    auto otherTipRef = dynamic_cast<const TipRef *>(&other);
    if(!otherTipRef) {
        return false;
    }

    return *arguments.front() == *otherTipRef->getAddressOfField();
}

bool TipRef::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream& TipRef::print(std::ostream &out) const {
    out << "&" << *arguments.front();
    return out;
}

std::shared_ptr<TipType> TipRef::getAddressOfField() const{
    return arguments.front();
}

