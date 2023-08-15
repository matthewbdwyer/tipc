#include "PolyTypeConstraintCollectVisitor.h"
#include "ConstraintCollector.h"

PolyTypeConstraintCollectVisitor::PolyTypeConstraintCollectVisitor(
    SymbolTable *pTable, CallGraph *callGraph, std::shared_ptr<Unifier> u)
    : PolyTypeConstraintVisitor(pTable, callGraph, u,
                                std::move(buildConstraintHandler())) {}

std::unique_ptr<ConstraintHandler>
PolyTypeConstraintCollectVisitor::buildConstraintHandler() {
  return std::make_unique<ConstraintCollector>();
}

std::vector<TypeConstraint> &
PolyTypeConstraintCollectVisitor::getCollectedConstraints() {
  auto handler = dynamic_cast<ConstraintCollector *>(constraintHandler.get());
  if (!handler) {
    assert(0); // LCOV_EXCL_LINE
  }
  return handler->getCollectedConstraints();
}
