#include "ConstraintCollector.h"

void ConstraintCollector::handle(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    collected.emplace_back(t1,t2);
}

std::vector<TypeConstraint> &ConstraintCollector::getCollectedConstraints() {
    return collected;
}
