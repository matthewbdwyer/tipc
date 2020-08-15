#pragma once

#include "AST.h"

class TipType {
public:
    virtual bool operator==(const TipType& other) const = 0;
    virtual bool operator!=(const TipType& other) const = 0;
    virtual ~TipType() = default;
    friend std::ostream& operator<<(std::ostream& os, const TipType& obj) {
        return obj.print(os);
    }

protected:
    virtual std::ostream& print(std::ostream &out) const = 0;

};


