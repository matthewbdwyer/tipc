#pragma once
#include "AST.h"
#include "TipType.h"
#include <iostream>
#include <string>
#include <memory>

class TipVar: public TipType {
public:
    TipVar() = default;
    TipVar(ASTNode * node);

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    ASTNode* getNode() const { return node; }

    void accept(TipTypeVisitor *visitor);

protected:
    std::ostream& print(std::ostream &out) const override;

    ASTNode * node;
};

