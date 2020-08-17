#pragma once
#include "ConstraintHandler.h"
#include "TypeConstraint.h"

#include <vector>

/*!
 * \class ConstraintCollector
 *
 * \brief A constraint handler to collect constraints.
 *
 * While you can always unify and assert type safety as constraints are generated, you
 * can also collect up all of the constraints and then unify that collection later. This
 * ConstraintHandler implementation can be used for simply collecting the constraints. There
 * are at least two reasons one might prefer collecting and then unifying. First, is to verify
 * that constraints are being generated properly in unit tests. Second, is for learning purposes.
 * That is, this implementation might help one better understand the typechecking algorithm.
 */
class ConstraintCollector: public ConstraintHandler {
public:
    void handle(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) override;
    std::vector<TypeConstraint>& getCollectedConstraints();
private:
    std::vector<TypeConstraint> collected;

};

