#pragma once

#include "TipCons.h"
#include "TipType.h"
#include <string>
#include <vector>
#include <ostream>

class TipRecord: public TipCons {
public:
    TipRecord() = delete;
    TipRecord(std::vector<std::shared_ptr<TipType>> inits,
              std::vector<std::string> names);
    std::vector<std::string> names;
    std::vector<std::shared_ptr<TipType>> inits;
    virtual int arity() override;

    std::ostream& print(std::ostream &out) const override;
    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;
};

