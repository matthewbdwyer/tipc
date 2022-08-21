#include "TypeConstraintCollectVisitor.h"
#include "ConstraintCollector.h"

TypeConstraintCollectVisitor::TypeConstraintCollectVisitor(SymbolTable *pTable)
  : TypeConstraintVisitor(pTable, std::move(buildConstraintHandler())) { }

std::unique_ptr<ConstraintHandler> TypeConstraintCollectVisitor::buildConstraintHandler() {
    return std::make_unique<ConstraintCollector>();
}

std::vector<TypeConstraint> &TypeConstraintCollectVisitor::getCollectedConstraints() {
    auto handler = dynamic_cast<ConstraintCollector *>(constraintHandler.get());
    if(!handler) {
        assert(0);  // LCOV_EXCL_LINE
    }
    return handler->getCollectedConstraints();
}
