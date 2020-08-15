#pragma once

#include "TipType.h"

/*!
 * \class TypeConstraint
 *
 * \brief A simple type constraint representation.
 */
class TypeConstraint {
public:
    TypeConstraint() = delete;
    TypeConstraint(std::shared_ptr<TipType> l, std::shared_ptr<TipType> r);

    std::shared_ptr<TipType> lhs;
    std::shared_ptr<TipType> rhs;
    bool operator==(const TypeConstraint& other) const;
    bool operator!=(const TypeConstraint& other) const;
    friend std::ostream& operator<<(std::ostream& os, const TypeConstraint& obj);
};

