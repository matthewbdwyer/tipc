#pragma once

#include "TipCons.h"
#include "TipType.h"
#include <string>
#include <vector>
#include <ostream>

/*!
 * \class TipRecord
 *
 * \brief A proper type representing a record
 */
class TipRecord: public TipCons {
public:
    TipRecord() = delete;
    TipRecord(std::vector<std::shared_ptr<TipType>> inits, std::vector<std::string> names);

    std::vector<std::string> const & getNames() const;
    std::vector<std::shared_ptr<TipType>>& getInits();
    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;

private:
    std::vector<std::string> const names;
};

