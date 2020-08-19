#include "ConstraintCollector.h"
#include <iostream>
#include "loguru.hpp"

namespace {
bool verbose = false;
}

void ConstraintCollector::handle(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    LOG_S(1) << "Generating constraint " << *t1 << " = " << *t2 << std::endl;
    collected.emplace_back(t1,t2);
}

std::vector<TypeConstraint> &ConstraintCollector::getCollectedConstraints() {
    return collected;
}
