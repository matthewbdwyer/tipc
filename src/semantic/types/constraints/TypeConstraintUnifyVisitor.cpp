#include "TypeConstraintUnifyVisitor.h"
#include "ConstraintUnifier.h"

TypeConstraintUnifyVisitor::TypeConstraintUnifyVisitor(SymbolTable *pTable)
    : TypeConstraintVisitor(pTable, std::move(buildConstraintHandler())) {}

std::shared_ptr<ConstraintHandler>
TypeConstraintUnifyVisitor::buildConstraintHandler() {
  return std::make_shared<ConstraintUnifier>();
}
