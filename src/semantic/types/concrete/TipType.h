#pragma once

#include "AST.h"
#include <set>

class TipVar;

class TipType {
public:
    virtual bool operator==(const TipType& other) const = 0;
    virtual bool operator!=(const TipType& other) const = 0;
    virtual ~TipType() = default;
    friend std::ostream& operator<<(std::ostream& os, const TipType& obj) {
        return obj.print(os);
    }

    /*! \brief Return the set of free type variables in this type.
     */
    virtual std::set<std::shared_ptr<TipVar>> freeVars() const = 0;

    /*! \brief Substitute t for v in this type.
     *
     * Perform an in-place update of this type replacing all occurrences of variable v
     * with type t.
     * \param v The variable to be replaced.
     * \param t The type substitution.
     */
    virtual void subst(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t) = 0;

protected:
    virtual std::ostream& print(std::ostream &out) const = 0;

};


