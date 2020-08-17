#include "TipCons.h"
#include "TipMu.h"
#include "TipAlpha.h"
#include "UnificationError.h"
#include "Unifier.h"
#include "Substituter.h"
#include "TypeVars.h"
#include <iostream>
#include <sstream>
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

    if(isVar(rep1) && isVar(rep2)) {
        unionFind->quick_union(rep1, rep2);
    } else if(isVar(rep1) && isProperType(rep2)) {
        unionFind->quick_union(rep1, rep2);
    } else if(isProperType(rep1) && isVar(rep2)) {
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

namespace {
  bool verbose = false;
}

/*! \fn close
 *  \brief Close a type expression replacing all variables with primitives.
 *
 * The method uses the solution to the type equations stored in the union-find
 * structure after solving.  It also makes use of two helper classes to
 * perform substitutions of variables and to identify the free variables in
 * the type expression (i.e., the one's not bound in mu quantifiers).
 * \sa Substituter
 * \sa TypeVars
 */
std::shared_ptr<TipType> Unifier::close(
        std::shared_ptr<TipType> type, std::set<std::shared_ptr<TipVar>> visited) {

  if (isVar(type)) {
    auto v = std::dynamic_pointer_cast<TipVar>(type);

    if (verbose) {
      std::cout << "Unifier closing variable: " << *v << std::endl;
    }

    auto wasVisited = 
            std::find(visited.begin(), visited.end(), v) != visited.end();
    if (wasVisited && (unionFind->find(type) != v)) {
      // No cyclic reference to v and it does not map to itself
      visited.insert(v);
      auto closedV = close(unionFind->find(type), visited);

      // If the variable is an alpha, then reuse it else create a new
      // alpha with the node.
      auto newV = (isAlpha(v)) ? v : std::make_shared<TipAlpha>(v->getNode());
      auto freeV = TypeVars::collect(closedV.get());
      if (freeV.find(newV) == freeV.end()) {
        // No cyclic reference in closed type
        return closedV;
      } else {
        // Cyclic reference requires a mu type constructor
        auto substClosedV = Substituter::substitute(closedV.get(), v.get(), newV);
        return std::make_shared<TipMu>(newV, substClosedV);
      }
    } else {
      // Unconstrained type variable
      return std::make_shared<TipAlpha>(v->getNode());
    } 

  } else if (isCons(type)) {
    auto c = std::dynamic_pointer_cast<TipCons>(type);

    if (verbose) {
      std::cout << "Unifier closing constructor: " << *c << std::endl;
    }

    // close each argument of the constructor for each free variable
    auto freeV = TypeVars::collect(c.get());

    if (verbose) {
      std::cout << "Found free variables: " << std::endl;
      for (auto v : freeV) {
        std::cout << "  " << *v << std::endl;
      }
    }

    std::vector<std::shared_ptr<TipType>> temp;
    auto current = c->getArguments();
    for (auto v : freeV) {
      for (auto a : current) {
         if (verbose) {
           std::cout << "Closing " << *v << std::endl;
         }
         auto closedV = close(unionFind->find(v), visited);
         if (verbose) {
           std::cout << "Done closing " << *v << " with " << *closedV << std::endl;
         }

         if (verbose) {
           std::cout << "Substituting in " << *a << ": " << *v << " for " << *closedV << std::endl;
         }
         auto subst = Substituter::substitute(a.get(), v.get(), closedV);
         if (verbose) {
           std::cout << "Done substituting  in " << *a << " yielding " << *subst << std::endl;
         }
         temp.push_back(subst);
      }
      current = temp;
    }

    // replace arguments with current
    c->setArguments(current);

    if (verbose) {
      std::cout << "Unifier done closing constructor with " << *c << std::endl;
    }
 
    return c;

  } else if (isMu(type)) {
    auto m = std::dynamic_pointer_cast<TipMu>(type);

    if (verbose) {
      std::cout << "Unifier closing mu: " << *m << std::endl;
    }

    return std::make_shared<TipMu>(m->getV(), close(m->getT(), visited));

  } 

  // TBD : I think this is unreachable
  return type;
}

/*! \brief Looks up the inferred type in the type solution.
 *
 * Here we want to produce an inferred type that is "closed" in the
 * sense that all variables in the type definition are replaced with
 * their base types.
 */ 
std::shared_ptr<TipType> Unifier::inferred(std::shared_ptr<TipVar> v) {
  std::set<std::shared_ptr<TipVar>> visited;
  return close(unionFind->find(v), visited);
}

void Unifier::throwUnifyException(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    std::stringstream s;
    s << "Type error cannot unify " << *t1 << " and " << *t2 <<
        " (respective roots are: " << *unionFind->find(t1) << " and " <<
        *unionFind->find(t2) << ")";
    throw UnificationError(s.str().c_str());
}

bool Unifier::isVar(std::shared_ptr<TipType> type) {
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

bool Unifier::isAlpha(std::shared_ptr<TipType> type) {
    return std::dynamic_pointer_cast<TipAlpha>(type) != nullptr;
}

