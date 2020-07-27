#pragma once
#include "AST.h"
#include "TipType.h"
#include <iostream>
#include <string>

class TipVar: public TipType {
public:
    TipVar() = default;
    TipVar(AST::Node * node);

    AST::Node * node;
    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;
    std::ostream& print(std::ostream &out) const override;
};

