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

class Unifier {
public:
    Unifier() = delete;
    Unifier(std::vector<TypeConstraint>);
    ~Unifier();

    void solve();
    void unify(TipType * t1, TipType * t2);
private:
    static bool isTypeVariable(TipType *TipType);
    static bool isProperType(TipType *);
    static bool isCons(TipType *);
    void throwUnifyException(TipType * TipType1, TipType * TipType2);
    std::vector<TypeConstraint> constraints;
    std::unique_ptr<UnionFind<TipType>> unionFind;
};

