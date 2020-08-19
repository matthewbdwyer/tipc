#pragma once

#include "TipType.h"
#include "TipVar.h"
#include <string>

/*!
 * \class TipMu
 *
 * \brief Class representing a recursive type
 */
class TipMu: public TipType {
public:
    TipMu() = delete;
    TipMu(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t);

    const std::shared_ptr<TipVar> &getV() const;
    const std::shared_ptr<TipType> &getT() const;

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;

private:
    std::shared_ptr<TipVar> v;
    std::shared_ptr<TipType> t;
};


