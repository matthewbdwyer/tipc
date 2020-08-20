#pragma once

#include "TipTypeVisitor.h"
#include <set>

/*! \brief Produces set of type variables in a type expression.
 */
class TypeVars: public TipTypeVisitor {
  std::set<std::shared_ptr<TipVar>> vars;
public:
  TypeVars() = default;

  /*! \brief Collect the set of type variables in a type expression.
   *
   * \param t The type within which to collect variables.
   * \return The set of type variables.
   */
  static std::set<std::shared_ptr<TipVar>> collect(TipType* t);

  std::set<std::shared_ptr<TipVar>> getVars() { return vars; }

  virtual void endVisit(TipMu * element) override;
  virtual void endVisit(TipAlpha * element) override;
  virtual void endVisit(TipVar * element) override;
};

