#pragma once

#include "TipType.h"
#include <vector>

/*! \brief Base type for all type constructors.
 *
 * This virtual type adds common functionality for composite types
 * that have type sub-expressions.  
 */
class TipCons: public TipType {
public:
    TipCons() = default;

    const std::vector<std::shared_ptr<TipType>> &getArguments() const;
    void setArguments(std::vector<std::shared_ptr<TipType>> &args);
    virtual int arity() const;
    bool doMatch(TipType const * t) const;

    // delegate the obligation to override accept to subtypes

protected:
    TipCons(std::vector<std::shared_ptr<TipType>> arguments);
    std::vector<std::shared_ptr<TipType>> arguments ;
};

