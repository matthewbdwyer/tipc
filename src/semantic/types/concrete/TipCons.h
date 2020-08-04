#pragma once

#include "TipType.h"
#include <set>
#include <vector>

class TipCons: public TipType {
public:
    TipCons() = default;

    const std::vector<std::shared_ptr<TipType>> &getArguments() const;
    virtual int arity() const;
    bool doMatch(TipType * t) const;

protected:
    TipCons(std::vector<std::shared_ptr<TipType>> arguments);
    std::vector<std::shared_ptr<TipType>> arguments ;
};

