#pragma once

#include "TipType.h"
#include "TipCons.h"
#include <string>

class TipInt: public TipCons {
public:
    TipInt();

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;
    std::ostream& print(std::ostream &out) const override;
};

