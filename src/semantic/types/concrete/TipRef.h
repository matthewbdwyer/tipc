#pragma once
#include "TipType.h"
#include "TipCons.h"

class TipRef: public TipCons {
public:
    TipRef() = delete;
    TipRef(std::shared_ptr<TipType> of);

    std::shared_ptr<TipType> getAddressOfField() const;

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

protected:
    std::ostream& print(std::ostream &out) const override;
};


