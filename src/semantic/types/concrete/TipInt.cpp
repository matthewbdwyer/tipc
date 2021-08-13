#include "TipInt.h"
#include "TipTypeVisitor.h"

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
}  // LCOV_EXCL_LINE

// TipInt is a 0-ary type constructor so it has no arguments to visit
void TipInt::accept(TipTypeVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}
