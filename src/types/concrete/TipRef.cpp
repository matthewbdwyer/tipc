#include "TipRef.h"
#include <sstream>

TipRef::TipRef(std::shared_ptr<TipType> of): of(of) {
    // TODO: Not yet implemented
    //arguments.push_back(of.get());
}

bool TipRef::operator==(const TipType &other) const {
    if(auto t = dynamic_cast<const TipRef *>(&other)) {
        return *of == *(t->of);
    }
    return false;
}

bool TipRef::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream& TipRef::print(std::ostream &out) const {
    out << "&" << *of;
    return out;
}

