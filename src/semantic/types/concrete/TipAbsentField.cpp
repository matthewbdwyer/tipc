#include "TipAbsentField.h"
#include "TipTypeVisitor.h"

#include <string>

TipAbsentField::TipAbsentField() { }

bool TipAbsentField::operator==(const TipType &other) const {
    auto otherTipAbsentField = dynamic_cast<TipAbsentField const *>(&other);
    if(!otherTipAbsentField) {
        return false;
    }

    return true;
}

bool TipAbsentField::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipAbsentField::print(std::ostream &out) const {
    out << std::string("\u25C7");
    return out;
}  // LCOV_EXCL_LINE

// TipAbsentField is a 0-ary type constructor so it has no arguments to visit
void TipAbsentField::accept(TipTypeVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}
