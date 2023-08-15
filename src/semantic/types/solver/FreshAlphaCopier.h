#pragma once

#include "Copier.h"

/*! \brief Makes a copy of a TipType with fresh alphas for the given ASTnode
 *
 * This subtype of the Copier overrides the behavior for
 * TipAlpha to just create a context-specific version of the alpha.
 */
class FreshAlphaCopier : public Copier {
public:
  FreshAlphaCopier() = default;

  /*! Copy the type replacing all alpha nodes with context-specific alphas
   */
  static std::shared_ptr<TipType> copy(TipType *t, ASTNode *context);

  virtual void endVisit(TipAlpha *element) override;

  ASTNode *context;
};
