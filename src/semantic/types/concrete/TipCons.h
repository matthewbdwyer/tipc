#pragma once

#include "TipType.h"
#include <set>
#include <vector>

class TipCons: public TipType {
public:
    TipCons() = default;

    std::set<std::shared_ptr<TipVar>> freeVars() const override;
    void subst(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t) override;

    const std::vector<std::shared_ptr<TipType>> &getArguments() const;
    virtual int arity() const;
    bool doMatch(TipType const * t) const;

protected:
    TipCons(std::vector<std::shared_ptr<TipType>> arguments);
    std::vector<std::shared_ptr<TipType>> arguments ;
};

