#pragma once
#include "AST.h"
#include "TipType.h"
#include <iostream>
#include <string>
#include <memory>

/*!
 * \class TipVar
 *
 * \brief Class representing a type variable
 */
class TipVar: public TipType {
public:
    TipVar() = default;
    TipVar(ASTNode * node);

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    ASTNode* getNode() const { return node; }

    void accept(TipTypeVisitor *visitor) override;

protected:
    //! \brief Type variables printed as ASTNode@line:col
    std::ostream& print(std::ostream &out) const override;

    ASTNode * node;
};

