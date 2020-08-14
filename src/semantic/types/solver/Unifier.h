#pragma once

#include "TipCons.h"
#include "TipType.h"
#include "TipVar.h"
#include "TypeConstraint.h"
#include "UnionFind.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

/*! \brief Solve type constraints using term-unification.
 *
 * Constraints are collected * ahead-of-time and then processed collectively 
 * performing term-unification.
 *
 * A possible extension is to unify constraints on-the-fly as they are collected.
 */
class Unifier {
public:
    Unifier() = delete;

    //! \brief Constructor for solving constraints collected ahead-of-time
    Unifier(std::vector<TypeConstraint>);

    ~Unifier();

    //! \brief Solve a set of collected constraints
    void solve();

    //! \brief Unify the terms representing two types
    void unify(TipType * t1, TipType * t2);

private:
    static bool isTypeVariable(TipType *TipType);
    static bool isProperType(TipType *);
    static bool isCons(TipType *);
    void throwUnifyException(TipType * TipType1, TipType * TipType2);
    std::vector<TypeConstraint> constraints;
    std::unique_ptr<UnionFind<TipType>> unionFind;
};

