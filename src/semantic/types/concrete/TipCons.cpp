#include "TipCons.h"
#include "TipTypeVisitor.h"

int TipCons::arity() const {
    return arguments.size();
}

/*! \brief Check for dynamic subtype and artity agreement
 * We use a crude dynamic type test here that is not robust to
 * the addition of new subtypes of TipCons.  Extend this if you
 * add such a subtype.
 */
bool TipCons::doMatch(TipType const * t) const {
    // Check if they are both TipFunction
    if ((dynamic_cast<TipFunction const *>(t) &&
         dynamic_cast<TipFunction const *>(this)) ||
        // or both TipInt
        (dynamic_cast<TipInt const *>(t) &&
         dynamic_cast<TipInt const *>(this)) ||
        // or both TipRecords
        (dynamic_cast<TipRecord const *>(t) &&
         dynamic_cast<TipRecord const *>(this)) ||
        // or both TipRefs
        (dynamic_cast<TipRef const *>(t) &&
         dynamic_cast<TipRef const *>(this))) {
      auto tipCons = dynamic_cast<TipCons const *>(t);
      return tipCons->arity() == arity();
    }
    return false;
}

TipCons::TipCons(std::vector<std::shared_ptr<TipType>> arguments) : arguments(std::move(arguments)) { }

void TipCons::setArguments(std::vector<std::shared_ptr<TipType>> &a) {
    arguments = a;
}

const std::vector<std::shared_ptr<TipType>> &TipCons::getArguments() const {
    return arguments;
}
