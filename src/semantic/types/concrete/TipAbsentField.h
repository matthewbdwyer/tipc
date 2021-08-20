#pragma once

#include "TipType.h"
#include "TipCons.h"
#include <string>

/*!
 * \class TipAbsentField
 *
 * \brief A proper type representing an absent record field 
 */
class TipAbsentField: public TipCons {
public:
    TipAbsentField();

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;
};

