#pragma once

#include "Substituter.h"

/*! \brief Makes a copy of a TipType
 *
 * This subtype of the Substituter overrides the behavior for TipVar
 * and TipAlpha to just copy that node rather than perform a substitution.
 */
class Copier : public Substituter {
public:
  Copier() = default;

  static std::shared_ptr<TipType> copy(std::shared_ptr<TipType> s);

  virtual void endVisit(TipAlpha *element) override;
  virtual void endVisit(TipVar *element) override;
};
