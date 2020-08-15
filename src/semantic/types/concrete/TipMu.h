#pragma once

#include <string>
#include "TipType.h"
#include "TipVar.h"

class TipMu: public TipType {
public:
    TipMu() = delete;
    TipMu(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t);

    std::ostream& print(std::ostream &out) const override;
    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    std::shared_ptr<TipVar> getV() const { return v; }
    std::shared_ptr<TipVar> getT() const { return t; }

private:
    std::shared_ptr<TipVar> v;
    std::shared_ptr<TipType> t;

protected:
    std::ostream& print(std::ostream &out) const override;
};


