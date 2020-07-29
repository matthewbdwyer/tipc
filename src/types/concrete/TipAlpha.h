#pragma once

#include "TipVar.h"
#include "TipCons.h"
#include <string>

class TipAlpha: public TipVar {
public:
    TipAlpha() = default;
    TipAlpha(std::string x);
    std::ostream& print(std::ostream &out) const override;
    virtual bool operator==(const TipType& other) const override;
    virtual bool operator!=(const TipType& other) const override;
    friend std::ostream& operator<<(std::ostream& os, const TipType& obj);

private:
    std::string x;

};


