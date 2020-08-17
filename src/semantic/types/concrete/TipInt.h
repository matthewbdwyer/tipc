#pragma once

#include "TipType.h"
#include "TipCons.h"
#include <string>

class TipInt: public TipCons {
public:
    TipInt();

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor);

protected:
    std::ostream& print(std::ostream &out) const override;
};

