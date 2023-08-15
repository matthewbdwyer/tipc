#pragma once

#include "CallGraph.h"
#include "TypeConstraintVisitor.h"
#include "Unifier.h"

/*! \class PolyTypeConstraintVisitor
 *
 *  \brief Visitor generates polymorphic type constraints and collects them.
 *  This visitor is called for a function with the requirement that all
 *  called functions already have a generalized type computed in the given
 *  function polymorphic type inference map.
 */
class PolyTypeConstraintVisitor : public TypeConstraintVisitor {
public:
  explicit PolyTypeConstraintVisitor(
      SymbolTable *pTable, CallGraph *callGraph, std::shared_ptr<Unifier> u,
      std::unique_ptr<ConstraintHandler> handler);

  virtual void endVisit(ASTFunAppExpr *element) override;

private:
  CallGraph *callGraph;
  std::shared_ptr<Unifier> unifier;
};
