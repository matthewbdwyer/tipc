#pragma once
#include "TipCons.h"
#include "TipType.h"

/*!
 * \class TipRef
 *
 * \brief A proper type representing a reference
 */
class TipRef : public TipCons {
public:
  TipRef() = delete;
  TipRef(std::shared_ptr<TipType> of);

  std::shared_ptr<TipType> getReferencedType() const;

  bool operator==(const TipType &other) const override;
  bool operator!=(const TipType &other) const override;

  void accept(TipTypeVisitor *visitor) override;

protected:
  std::ostream &print(std::ostream &out) const override;
};
