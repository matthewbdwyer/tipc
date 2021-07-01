#include "TypeConstraint.h"

#include "iostream"

TypeConstraint::TypeConstraint(std::shared_ptr<TipType> l, std::shared_ptr<TipType> r): lhs(l), rhs(r) {};

/**
 *  \brief Two constraints are equal if their underlying terms are equal.
 *
 */
bool TypeConstraint::operator==(const TypeConstraint &other) const {
    return *lhs == *(other.lhs);
}

bool TypeConstraint::operator!=(const TypeConstraint &other) const {
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const TypeConstraint &obj) {
    return os << *obj.lhs << " = " << *obj.rhs;
}
