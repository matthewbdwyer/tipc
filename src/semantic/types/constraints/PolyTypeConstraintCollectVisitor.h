#pragma once

#include "ASTVisitor.h"
#include "ConstraintHandler.h"
#include "PolyTypeConstraintVisitor.h"
#include "SymbolTable.h"
#include "TipType.h"
#include "TypeConstraint.h"
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <vector>

/*! \class PolyTypeConstraintCollectVisitor
 *
 *  \brief Visitor generates type constraints and collects them.
 */
class PolyTypeConstraintCollectVisitor : public PolyTypeConstraintVisitor {
public:
  explicit PolyTypeConstraintCollectVisitor(SymbolTable *pTable,
                                            CallGraph *callGraph,
                                            std::shared_ptr<Unifier> u);
  std::vector<TypeConstraint> &getCollectedConstraints();

private:
  static std::unique_ptr<ConstraintHandler> buildConstraintHandler();
};
