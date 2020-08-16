#include "TipCons.h"
#include "TipMu.h"
#include "TipAlpha.h"
#include "UnificationError.h"
#include "Unifier.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

Unifier::Unifier() : unionFind(std::move(std::make_unique<UnionFind>())) {}

Unifier::Unifier(std::vector<TypeConstraint> constrs) : constraints(std::move(constrs)) {
    std::vector<std::shared_ptr<TipType>> types;
    for(TypeConstraint& constraint : constraints) {
        auto lhs = constraint.lhs;
        auto rhs = constraint.rhs;
        types.push_back(lhs);
        types.push_back(rhs);

        if(auto f1 = std::dynamic_pointer_cast<TipCons>(lhs)) {
            for(auto &a : f1->getArguments()) {
                types.push_back(a);
            }
        }
        if(auto f2 = std::dynamic_pointer_cast<TipCons>(rhs)) {
            for(auto &a : f2->getArguments()) {
                types.push_back(a);
            }
        }
    }

    unionFind = std::make_unique<UnionFind>(types);
}

void Unifier::solve() {
    for(TypeConstraint &constraint: constraints) {
        unify(constraint.lhs, constraint.rhs);
    }
}

void Unifier::unify(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    auto rep1 = unionFind->find(t1);
    auto rep2 = unionFind->find(t2);

    if(rep1 == rep2) {
       return;
    }

    if(isTypeVariable(rep1) && isTypeVariable(rep2)) {
        unionFind->quick_union(rep1, rep2);
    } else if(isTypeVariable(rep1) && isProperType(rep2)) {
        unionFind->quick_union(rep1, rep2);
    } else if(isProperType(rep1) && isTypeVariable(rep2)) {
        unionFind->quick_union(rep2, rep1);
    } else if(isCons(rep1) && isCons(rep2)) {
        auto f1 = std::dynamic_pointer_cast<TipCons>(rep1);
        auto f2 = std::dynamic_pointer_cast<TipCons>(rep2);
        if(!f1->doMatch(f2.get())) {
            throwUnifyException(t1,t2);
        }

        unionFind->quick_union(rep1, rep2);
        for(int i = 0; i < f1->getArguments().size(); i++) {
            auto a1 = f1->getArguments().at(i);
            auto a2 = f2->getArguments().at(i);
            unify(a1, a2);
        }
    } else {
        throwUnifyException(t1,t2);
    }
}


/*
NICK : this logic should be migrated to the methods in the TipType
hierarchy.  I added the declarations in TipType, but didn't go further.


// When this is moved to TipType hierarchy this should be a void
// function that makes the substitutions "in place".  That is we
// should modify the arguments in the TipCons rather than completely
// reconstructing the TipCons.  If we can
// do it this way then we can treat all TipCons subtypes the same.
//
// At least this would be the simplest way to go if it works.
// The other choice is to build some general TipType visitor 
// mechanism that can rebuild them from the ground up to perform
// the substitution.  In this case we have to handle substitution 
// for all of the TipTypes (I think)
std::shared_ptr<TipType> subst(std::shared_ptr<TipType> t,
                               std::shared_ptr<TipVar> v,
                               std::shared_ptr<TipType> s) {
  if (isVariable(t)) {
    if (t == v) {
      return s;
    } 
  } else if (isCons(t)) {
    auto c = std::dynamic_pointer_cast<TipCons>(t);
     
    for (auto a : c->getArguments()) {
      for (auto v : freeVars(a)) {
        free.insert(v);
      } 
    }
  } else if (isMu(t)) {
  } 

  // No substitution needed
  return t;
}

std::set<std::shared_ptr<TipVar>> freeVars(std::shared_ptr<TipType> t) {
  std::set<std::shared_ptr<TipVar>> free;

  if (isTypeVariable(t)) {
    free.insert(t);

  } else if (isCons(t)) {
    auto c = std::dynamic_pointer_cast<TipCons>(t);
    // map through the constructor to accumulate the variables
    for (auto a : c->getArguments()) {
      for (auto v : freeVars(a)) {
        free.insert(v);
      } 
    }

  } else if (isMu(t)) {
    auto m = std::dynamic_pointer_cast<TipMu>(t);
    for (auto v : freeVars(m->getT())) {
      free.insert(v);
    } 
  } 

  return free;
}
*/

/*! \fn close
 *  \brief Close a type expression replacing all type variables with primitives.
 *
 * Closure uses the solution to the type equations stored in the union-find
 * structure after solving.  
 *
 */

/*
std::shared_ptr<TipType> Unifier::close(std::shared_ptr<TipType> type,
                              std::vector<std::shared_ptr<TipVar>> visited) {
  if (isTypeVariable(type)) {
    auto v = std::dynamic_pointer_cast<TipVar>(type);
    auto wasVisited = std::find(visited.begin(), visited.end(), v) != visited.end();
    if (wasVisited && (unionFind->find(v) != v)) {
      // No cyclic reference to v and it does not map to itself
      visited.push_back(v);
      auto closedV = close(unionFind->find(v), visited);

// This is inherited from the Scala code
// I don't understand how checking for newV in closedV->freeVars() detects a cycle

      auto newV = std::shared_ptr<TipAlpha>(v);
      if (closedV->freeVars().find(newV) == visited.end()) {
        // No cyclic reference in closed type
        return closedV;
      } else {
        // Cyclic reference requires a mu type constructor
        closedV->subst(v, newV);
        return std::shared_ptr<TipMu>(newV, closedV);
      }
    } else {
      // Unconstrained quantified type variable
      return std::shared_ptr<TipAlpha>(v);
    } 
  } else if (isCons(type)) {
    auto c = std::dynamic_pointer_cast<TipCons>(type);
    // close each argument of the constructor

// Scala code that needs to be mapped to our data types
//c.fv.foldLeft(t: Term[A]) { (acc, v) =>
//  acc.subst(v, closeRec(v, env, visited))
//}
///


  } else if (isMu(type)) {
    return std::shared_ptr<TipMu>(m.v, close(m.t, visited));
  } else {
    return type;
  }
};
*/

/*! \brief Looks up the inferred type in the type solution.
 *
 * Here we want to produce an inferred type that is "closed" in the
 * sense that all variables in the type definition are replaced with
 * their base types.
 *
 * TBD: When we have self-referential types we need a Mu
 */ 
std::shared_ptr<TipType> Unifier::inferred(std::shared_ptr<TipVar> v) {
  std::vector<std::shared_ptr<TipVar>> visited;
  // TODO
  //auto vType = close(unionFind->find(v), visited);
  //return vType;
}

void Unifier::throwUnifyException(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    std::stringstream s;
    s << "Type error cannot unify " << *t1 << " and " << *t2 <<
        " (respective roots are: " << *unionFind->find(t1) << " and " <<
        *unionFind->find(t2) << ")";
    throw UnificationError(s.str().c_str());
}

bool Unifier::isTypeVariable(std::shared_ptr<TipType> type) {
    return std::dynamic_pointer_cast<TipVar>(type) != nullptr;
}

bool Unifier::isProperType(std::shared_ptr<TipType> type) {
    return std::dynamic_pointer_cast<TipVar>(type) == nullptr;
}

bool Unifier::isCons(std::shared_ptr<TipType> type) {
    return std::dynamic_pointer_cast<TipCons>(type) != nullptr;
}

bool Unifier::isMu(std::shared_ptr<TipType> type) {
    return std::dynamic_pointer_cast<TipMu>(type) != nullptr;
}

