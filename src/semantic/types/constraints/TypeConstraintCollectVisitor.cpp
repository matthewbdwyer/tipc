#include "TypeConstraintCollectVisitor.h"
#include "ConstraintCollector.h"

TypeConstraintCollectVisitor::TypeConstraintCollectVisitor(SymbolTable *pTable)
    : TypeConstraintVisitor(pTable, std::move(buildConstraintHandler())) {}

std::shared_ptr<ConstraintHandler>
TypeConstraintCollectVisitor::buildConstraintHandler() {
  return std::make_shared<ConstraintCollector>();
}

std::vector<TypeConstraint> &
TypeConstraintCollectVisitor::getCollectedConstraints() {
  auto handler = dynamic_cast<ConstraintCollector *>(constraintHandler.get());
  if (!handler) {
    assert(0); // LCOV_EXCL_LINE
  }
  return handler->getCollectedConstraints();
}
