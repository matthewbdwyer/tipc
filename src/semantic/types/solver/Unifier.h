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
#include "UnionFind.h"
#include <vector>

/*!
 * \class Unifier
 *
 * \brief Class used to solve type constraints and establish typability.
 *
 * Make uses of a union-find data structure. This class will throw a 
 * UnificationError anytime two terms cannot be unified, either because
 * their constructor or arity mismatch.
 */
class Unifier {
public:
    Unifier();
    explicit Unifier(std::vector<TypeConstraint>);
    ~Unifier() = default;

    void solve();
    void unify(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2);
 
    /*! \brief Returns the inferred type for a given type variable.
     *
     * Calls should follow a call to solve that produces a union find
     * structure reflecting the inferred set of types. 
     */
    std::shared_ptr<TipType> inferred(std::shared_ptr<TipVar> v);
private:
    static bool isCons(std::shared_ptr<TipType>);
    static bool isMu(std::shared_ptr<TipType> type);
    static bool isProperType(std::shared_ptr<TipType>);
    static bool isTypeVariable(std::shared_ptr<TipType>);
    void throwUnifyException(std::shared_ptr<TipType> TipType1, std::shared_ptr<TipType> TipType2);
    std::vector<TypeConstraint> constraints;
    std::unique_ptr<UnionFind> unionFind;
    //std::shared_ptr<TipType> close(std::shared_ptr<TipType> type, std::vector<std::shared_ptr<TipVar>> visited);

};

