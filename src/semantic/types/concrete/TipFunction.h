#pragma once

#include "TipCons.h"
#include "TipType.h"
#include <memory>
#include <string>
#include <vector>

/*!
 * \class TipFunction
 *
 * \brief A proper type representing a function
 */
class TipFunction : public TipCons {
public:
  TipFunction() = delete;
  TipFunction(std::vector<std::shared_ptr<TipType>> params,
              std::shared_ptr<TipType> ret);

  std::vector<std::shared_ptr<TipType>> getParamTypes() const;
  std::shared_ptr<TipType> getReturnType() const;

  bool operator==(const TipType &other) const override;
  bool operator!=(const TipType &other) const override;

  void accept(TipTypeVisitor *visitor) override;

protected:
  std::ostream &print(std::ostream &out) const override;

private:
  std::vector<std::shared_ptr<TipType>>
  combine(std::vector<std::shared_ptr<TipType>> params,
          std::shared_ptr<TipType> ret);
};
