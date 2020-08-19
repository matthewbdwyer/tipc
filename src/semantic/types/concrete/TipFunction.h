#pragma once

#include "TipType.h"
#include "TipCons.h"
#include <string>
#include <vector>
#include <memory>

/*!
 * \class TipFunction
 *
 * \brief A proper type representing a function
 */
class TipFunction: public TipCons {
public:
    TipFunction() = delete;
    TipFunction(std::vector<std::shared_ptr<TipType>> params, std::shared_ptr<TipType> ret);

    std::vector<std::shared_ptr<TipType>> getParams() const;
    std::shared_ptr<TipType> getReturnValue() const;

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;

private:
    std::vector<std::shared_ptr<TipType>> combine(std::vector<std::shared_ptr<TipType>> params, std::shared_ptr<TipType> ret);

};

