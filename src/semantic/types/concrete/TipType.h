#pragma once

#include <ostream>
#include <memory>

// Forward declare the visitor to resolve circular dependency
class TipTypeVisitor;

/*! \class TipType
 * \brief Abstract base class of all types
 *
 * Defines equality comparisons, output operator, and accept for visitor.
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


