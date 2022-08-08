#include "Unifier.h"
#include "Substituter.h"
#include "TipAlpha.h"
#include "TipCons.h"
#include "TipMu.h"
#include "TypeVars.h"
#include "UnificationError.h"

#include "loguru.hpp"

#include <iostream>
#include <sstream>
#include <utility>

/*
 * This code makes use of the dangerous combination of smart pointers and
 * standard libraries.  After several painful debugging sessions things are
 * working, but this code should be refactored to better integrate these.
 * We really need to use the underlying notion of equality on TipType to
 * perform the operations below and when using smart pointers we end up
 * having to do things explicitly while accessing and dereferencing the
 * managed pointer. 
 */

namespace { // Anonymous namespace for local helper functions

bool contains(std::set<std::shared_ptr<TipVar>> s, std::shared_ptr<TipVar> t) {
  for (auto e : s) {
    if (*e.get() == *t.get()) return true;
  } 
  return false;
}

std::string print(std::set<std::shared_ptr<TipVar>> varSet) {
  std::stringstream s;
  s << "{ ";
  for (auto v : varSet) {
    s << *v << " "; 
  }
  s << "}";
  return s.str();
}

}

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

/*! \fn unify
 *  \brief Attempts to unify the two type terms. Throws a UnificationError on failure.
 *
 * Unify is the core of the typechecking algorithm. It is very selective
 * about updating the underlying union-find graph. It enforces a number of requirements.
 * First, is that given a proper type and a type variable, the proper type
 * always becomes the canonical representative of the two terms. Next,
 * given two type variables, the canonical representation can be picked arbitrarily,
 * but it is, and must be, done consistently. Finally, given two proper types
 * the method enforces that they are the same. It does so by checking their arity
 * and then by unifying their subterms.
 *
 * The logic in this method is enough to conclude the type safety of a program. It
 * cannot however infer the types. For inference, see the close method.
 *
 * \sa t1
 * \sa t2
 */
void Unifier::unify(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    LOG_S(2) << "Unifying " << *t1 << " and " << *t2;

    auto rep1 = unionFind->find(t1);
    auto rep2 = unionFind->find(t2);

    LOG_S(2) << "Unifying with representatives " << *rep1 << " and " << *rep2;

    if(*rep1 == *rep2) {
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
        }  // LCOV_EXCL_LINE

        unionFind->quick_union(rep1, rep2);
        for(int i = 0; i < f1->getArguments().size(); i++) {
            auto a1 = f1->getArguments().at(i);
            auto a2 = f2->getArguments().at(i);
            unify(a1, a2);
        }
    } else {
        // This case should be unreachable
        assert(false); // LCOV_EXCL_LINE
    }

    LOG_S(2) << "Unifying representatives to " << *unionFind->find(t1);
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

    LOG_S(2) << "Close starting var " << *v << " with visited " << print(visited);

    if (!contains(visited, v) && (unionFind->find(type) != v)) {
      // No cyclic reference to v and it does not map to itself
      visited.insert(v);

      auto closedV = close(unionFind->find(type), visited);

      // If the variable is an alpha, then reuse it else create a new
      // alpha with the node.
      auto newV = (isAlpha(v)) ? v : std::make_shared<TipAlpha>(v->getNode());
      auto freeV = TypeVars::collect(closedV.get());
      if (contains(freeV,newV)) {
        // Cyclic reference requires a mu type constructor
        auto substClosedV = Substituter::substitute(closedV.get(), v.get(), newV);
        auto mu = std::make_shared<TipMu>(newV, substClosedV);

        LOG_S(2) << "Close making " << *mu << " to end var " << *v;
        return mu;

      } else {
        // No cyclic reference in closed type
        LOG_S(2) << "Close making " << *closedV << " to end var " << *v;
        return closedV;
      }
    } else {
      // Unconstrained type variable - should we start with fresh names to make output cleaner?
      auto alpha = std::make_shared<TipAlpha>(v->getNode());

      LOG_S(2) << "Close making " << *alpha << " to end var " << *v;
      return alpha;
    } 

  } else if (isCons(type)) {
    auto c = std::dynamic_pointer_cast<TipCons>(type);
    auto copy = Copier::copy(c);

    LOG_S(2) << "Close starting cons " << *c << " with visited " << print(visited);

    // close each argument of the constructor for each free variable
    auto freeV = TypeVars::collect(c.get());

    std::vector<std::shared_ptr<TipType>> temp;
    auto current = c->getArguments();
    for (auto v : freeV) {
      auto closedV = close(v, visited);
      for (auto a : current) {

    LOG_S(2) << "Close cons substituting " << *closedV << " for " << *v << " in " << *a;
        auto subst = Substituter::substitute(a.get(), v.get(), closedV);
    LOG_S(2) << "Close cons substitution yielded " << *subst;
        temp.push_back(subst);
      }
      current = temp;
      temp.clear();
    }

    // replace arguments with current
    c->setArguments(current);

    LOG_S(2) << "Close making " << *c << " to end cons " << *copy;

    return c;

  } else if (isMu(type)) {
    auto m = std::dynamic_pointer_cast<TipMu>(type);

    LOG_S(2) << "Close starting mu " << *m << " with visited " << print(visited);

    auto closedMu = std::make_shared<TipMu>(m->getV(), close(m->getT(), visited));

    LOG_S(2) << "Close making " << *closedMu << " to end mu " << *m;

    return closedMu;
  } 

  /* This should be unreachable since all subtypes of TipType are
   * subtypes of TipVar, TipCons, or TipMu -- the three cases above.
   * We could have left the final "else if" implicit above, but add the
   * assertion here to catch any problematic type extensions.  Note
   * that this necessitates the return statement.
   */
  assert(false);  // LCOV_EXCL_LINE
  return type;  // LCOV_EXCL_LINE
}

/*! \brief Looks up the inferred type in the type solution.
 *
 * Here we want to produce an inferred type that is "closed" in the
 * sense that all variables in the type definition are replaced with
 * their base types.  Because the close() function may destructively update
 * the unionFind structure, by generating new types, we save a copy
 * and restore it after closing.
 */ 
std::shared_ptr<TipType> Unifier::inferred(std::shared_ptr<TipType> v) {
  auto unionFindCopy = unionFind->copy();
  std::set<std::shared_ptr<TipVar>> visited;
  auto closedV = close(v, visited);
  unionFind = std::move(unionFindCopy);
  return closedV;
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

