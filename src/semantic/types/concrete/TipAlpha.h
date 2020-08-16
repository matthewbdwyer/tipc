#pragma once

#include "TipVar.h"
#include "TipCons.h"
#include <string>

class TipAlpha: public TipVar {
public:
    TipAlpha() = default;
    TipAlpha(std::string const av);

    std::string const & getAlphaValue() const;

    virtual bool operator==(const TipType& other) const override;
    virtual bool operator!=(const TipType& other) const override;

protected:
    std::ostream& print(std::ostream &out) const override;

private:
    std::string const alphaValue;
};


