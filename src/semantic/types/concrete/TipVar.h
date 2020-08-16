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
class TipVar: public TipType, std::enable_shared_from_this<TipVar> {
public:
    TipVar() = default;
    TipVar(ASTNode * node);

    std::set<std::shared_ptr<TipVar>> freeVars() const override;
    void subst(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t) override;

    ASTNode * node;
    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

protected:
    std::ostream& print(std::ostream &out) const override;
};

