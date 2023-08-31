#pragma once

#include "TipType.h"
#include "TipVar.h"
#include "TypeConstraint.h"
#include "UnionFind.h"
#include <set>
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

  /*! \brief Construct a Unifier with seeded with constraints.
   *
   * Useful for when one is collecting and then unifying constraints.
   */
  explicit Unifier(std::vector<TypeConstraint>);

  /*! \brief Construct an empty Unifier.
   *
   * Useful for when unifying on the fly.
   */
  ~Unifier() = default;

  /*! \brief Attempt to unify the two types
   * \throws UnificationError when constraints cannot be unifierd.
   */
  void unify(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2);

  /*! \brief Add constraints to this unifier.
   */
  void add(std::vector<TypeConstraint>);

  /*! \brief Solve the system of constraints that have presented to this
   * unifier. 
   * \pre The unifier has been constructed with seed values. 
   * Incremental solving can be achieved by adding constraints, via
   * the add method, after solving.  This will cause the new constraints
   * to be unified with the currently unified constraints.
   */
  void solve();

  /*! \brief Returns the inferred type for a given type.
   * \pre The unifier has computed a solution.
   * This will close the type by replacing any variables that
   * are bound to proper types in the inferred solution with that
   * proper type.
   */
  std::shared_ptr<TipType> inferred(std::shared_ptr<TipType> t);

  static bool isCons(std::shared_ptr<TipType> type);
  static bool isMu(std::shared_ptr<TipType> type);
  static bool isVar(std::shared_ptr<TipType> type);
  static bool isAlpha(std::shared_ptr<TipType> type);
  static bool isProperType(std::shared_ptr<TipType> type);

private:
  std::shared_ptr<TipType> close(std::shared_ptr<TipType> type,
                                 std::set<std::shared_ptr<TipVar>> visited);
  void throwUnifyException(std::shared_ptr<TipType> TipType1,
                           std::shared_ptr<TipType> TipType2);

  std::vector<TypeConstraint> constraints;
  std::shared_ptr<UnionFind> unionFind;
};
