#include "PolyTypeConstraintVisitor.h"
#include "FreshAlphaCopier.h"
#include "TypeVars.h"
#include "loguru.hpp"

PolyTypeConstraintVisitor::PolyTypeConstraintVisitor(
    SymbolTable *st, CallGraph* cg, std::shared_ptr<Unifier> u, 
    std::unique_ptr<ConstraintHandler> handler)
    : TypeConstraintVisitor(st, std::move(handler)), callGraph(cg), unifier(u) {};

/*! \brief Polymorphic type constraints for function application.
 *
 * Type Rules for "E(E1, ..., En)":
 *  [[E]] = ([[E1]], ..., [[En]]) -> [[E(E1, ..., En)]]
 *
 * Polymorphic typing handles the case where expression "E" has
 * a function type involving free type variables, i.e., \alpha.
 * In this case, rather than force such type variables to be unified
 * across all call sites, instead we create a copy of the function
 * type with unique free type variables, i.e., the instantiated type,
 * that can be unified with the function expression in the function application.
 *
 * An additional complexity is that we will perform this process for each
 * function that may be called from this call site.  Since unification solves
 * the conjunction of constraints this forces all of the corresponding free
 * type variables for each called function to be consistently typed at this call
 * site.  Different typings are possible at other call sites.
 *
 * FIX THE ABOVE ONCE THINGS ARE WORKING
 */
void PolyTypeConstraintVisitor::endVisit(ASTFunAppExpr  * element) {
  std::vector<std::shared_ptr<TipType>> actuals;
  for(auto &a : element->getActuals()) {
    actuals.push_back(astToVar(a));
  }
 
  /* For each called function:
   *   - if it is declared polymorphic then look up its generic type, insantiate it and unify
   *   - otherwise use the monomorphic approach
   */
  for (auto f : callGraph->getCalledFuns(element)) {
    auto fName = f->getName();
    auto fDecl = symbolTable->getFunction(fName);
    auto isPoly = symbolTable->getPoly(fName);

    if (isPoly) {
      auto genericType = unifier->inferred(astToVar(fDecl));
      auto copyType = FreshAlphaCopier::copy(genericType.get(), element);

      auto instantiatedType = std::dynamic_pointer_cast<TipFunction>(copyType);
      assert (instantiatedType != nullptr);

      LOG_S(1) << "Polymorphic type constraint for application of " << fName << " with generic type " << *genericType << " using instantiated type " << *instantiatedType;

      // Polymorphic function application 
      constraintHandler->handle(instantiatedType,
                                std::make_shared<TipFunction>(actuals, astToVar(element)));
    } else {
      // Monomorphic function application
      constraintHandler->handle(astToVar(element->getFunction()),
                                std::make_shared<TipFunction>(actuals, astToVar(element)));
    }
  } 
}
