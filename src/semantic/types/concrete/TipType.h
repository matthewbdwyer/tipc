#pragma once

#include <ostream>
#include <memory>

// Forward declare the visitor to resolve circular dependency
class TipTypeVisitor;

/*! \class TipType
 * \brief Abstract base class of all types
 *
 * Defines equality comparisons, output operator, and accept for visitor.
 * Type variables and operators, like mu, directly subtype TipType.
 * All other types, e.g., ints, functions, etc., are subtypes of TipCons,
 * since this allows type unification to just handle TipCons.  Consequently,
 * it means that if you want to extend the types supported you will need to
 * subtype TipCons.
 */
class TipType {
public:
    virtual bool operator==(const TipType& other) const = 0;
    virtual bool operator!=(const TipType& other) const = 0;
    virtual ~TipType() = default;
    friend std::ostream& operator<<(std::ostream& os, const TipType& obj) {
        return obj.print(os);
    }

    virtual void accept(TipTypeVisitor *visitor) = 0;

protected:
    virtual std::ostream& print(std::ostream &out) const = 0;

};


