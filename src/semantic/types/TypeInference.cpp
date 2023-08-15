#include "TypeInference.h"
#include "AbsentFieldChecker.h"
#include "PolyTypeConstraintCollectVisitor.h"
#include "TypeConstraint.h"
#include "TypeConstraintCollectVisitor.h"
#include "Unifier.h"
#include "loguru.hpp"
#include <memory>

/* Local name space for DFS visit variables */
namespace {
std::deque<ASTFunction *> sorted;
std::vector<ASTFunction *> unmarked;
} // namespace

/* DFS to compute call dependence assuming that the graph
 * does not have cycles.  The DFS updates the unmarked vector
 * which records the functions whose dependences have been computed.
 */
void topoVisit(CallGraph *cg, ASTFunction *f) {
  // If f is marked, i.e., not in unmarked, then backtrack DFS
  if (std::find(unmarked.begin(), unmarked.end(), f) == unmarked.end()) {
    return;
  }

  // remove it from unmarked
  auto fPosition = std::find(unmarked.begin(), unmarked.end(), f);
  assert(fPosition != unmarked.end()); // f must be in the unmarked list
  unmarked.erase(fPosition);

  // visit called functions
  for (auto c : cg->getCallees(f)) {
    topoVisit(cg, c);
  }

  // add it to sorted
  sorted.push_back(f);
}

/* Determine whether there is a call chain from function f to g.
 * To determine if a function is recursive call with f==g.
 */
bool mayIndirectlyCall(CallGraph *cg, ASTFunction *f, ASTFunction *g) {
  bool result = false;
  auto callees = cg->getCallees(f);
  for (auto c : callees) {
    if (c == g) {
      return true;
    } else {
      result = result || mayIndirectlyCall(cg, c, g);
    }
  }
  return result;
}

// Topologically sort the set of functions based on the call graph.
std::deque<ASTFunction *> topoSort(CallGraph *cg,
                                   std::vector<ASTFunction *> funcs) {
  /* Initialize globals for sort DFS */
  sorted = std::deque<ASTFunction *>();
  unmarked = funcs;

  while (!unmarked.empty()) {
    auto f = unmarked.back();
    topoVisit(cg, f);
  }
  return sorted;
}

std::vector<ASTFunction *> recursiveFuncs(CallGraph *cg) {
  auto funcs = cg->getVertices();
  auto recursiveFuncs = std::vector<ASTFunction *>();
  for (auto f : funcs) {
    if (mayIndirectlyCall(cg, f, f)) {
      recursiveFuncs.push_back(f);
    }
  }
  return recursiveFuncs;
}

/* Filters the call graph to eliminate any functions that call, either
 * directly or indirectly, a recursive function.
 * Returns a topological ordering of functions in the filtered graph.
 */
std::deque<ASTFunction *> topoSortNonRecursive(CallGraph *cg) {
  auto recFuncs = recursiveFuncs(cg);

  // Filter functions that directly or indirectly call a recursive function
  auto nonRecursiveFuncs = std::vector<ASTFunction *>();
  for (auto f : cg->getVertices()) {
    bool filter = false;
    for (auto r : recFuncs) {
      if (f == r) {
        filter = true;
      } else {
        filter = mayIndirectlyCall(cg, f, r);
      }
      if (filter)
        break;
    }

    if (!filter) {
      nonRecursiveFuncs.push_back(f);
    }
  }

  // Topologically sort the graph comprised of the non-recursive functions.
  return topoSort(cg, nonRecursiveFuncs);
}

/*
 * The returned unifier accounts for all of the program that is NOT
 * handled within the elements of the unifier map, i.e., is not
 * subjected to polymorphic type inference.
 */
std::shared_ptr<TypeInference> runPoly(ASTProgram *ast, SymbolTable *symbols,
                                       CallGraph *cg) {
  LOG_S(1) << "Generating Polymorphic Type Constraints";

  /* A single unifier is used for the staged polymorphic inference
   * and then the final monomorphic inference process.  The unifier
   * is solved after each stage, which corresponds to processing the
   * constraints of a non-recursive function.
   */
  auto unifier = std::make_shared<Unifier>();

  /* Generate and solve constraints for the non-recursive functions
   * in topological order for the call graph.
   */
  auto nonRecursiveFuncs = topoSortNonRecursive(cg);
  for (auto f : nonRecursiveFuncs) {
    LOG_S(1) << "Generating Polymorphic Type Constraints for " << *f;

    PolyTypeConstraintCollectVisitor polyVisitor(symbols, cg, unifier);
    f->accept(&polyVisitor);

    unifier->add(polyVisitor.getCollectedConstraints());
    unifier->solve();
  }

  LOG_S(1) << "Generating Residual Monomorphic Type Constraints";

  /* Iterate over functions those that are recursive, or that may directly
   * or indirectly call a recursive function, generate their constraints.
   */
  for (auto f : cg->getVertices()) {
    // Skip the functions for which polymorphic inference was applied
    if (std::find(nonRecursiveFuncs.begin(), nonRecursiveFuncs.end(), f) ==
        nonRecursiveFuncs.end()) {
      TypeConstraintCollectVisitor monoVisitor(symbols);
      f->accept(&monoVisitor);
      unifier->add(monoVisitor.getCollectedConstraints());
    }
  }

  /* Solve monomorphic constraints in combination with the
   * previously collected polymorphic constraints.
   */
  unifier->solve();

  AbsentFieldChecker::check(ast, unifier.get());

  return std::make_shared<TypeInference>(symbols, unifier);
}

/*
 * Performs monomorphic type inference on the entire program.
 */
std::shared_ptr<TypeInference> runMono(ASTProgram *ast, SymbolTable *symbols) {
  LOG_S(1) << "Generating Monomorphic Type Constraints";

  TypeConstraintCollectVisitor visitor(symbols);
  ast->accept(&visitor);

  LOG_S(1) << "Solving type constraints";

  auto unifier = std::make_shared<Unifier>(visitor.getCollectedConstraints());
  unifier->solve();

  AbsentFieldChecker::check(ast, unifier.get());

  return std::make_shared<TypeInference>(symbols, unifier);
}

/*
 * This implementation collects the constraints and then solves them with a
 * unifier instance.  The unifier then records the inferred type results that
 * can be subsequently queried.
 */
std::shared_ptr<TypeInference> TypeInference::run(ASTProgram *ast, bool doPoly,
                                                  CallGraph *cg,
                                                  SymbolTable *symbols) {
  return (doPoly) ? runPoly(ast, symbols, cg) : runMono(ast, symbols);
}

std::shared_ptr<TipType> TypeInference::getInferredType(ASTDeclNode *node) {
  auto var = std::make_shared<TipVar>(node);
  return unifier->inferred(var);
};

void TypeInference::print(std::ostream &s) {
  s << "\nFunctions : {\n";
  auto skip = true;
  for (auto f : symbols->getFunctions()) {
    if (skip) {
      skip = false;
      s << "  " << f->getName() << " : " << *getInferredType(f);
      continue;
    }
    s << ",\n  " + f->getName() << " : " << *getInferredType(f);
  }
  s << "\n}\n";

  for (auto f : symbols->getFunctions()) {
    s << "\nLocals for function " + f->getName() + " : {\n";
    skip = true;
    for (auto l : symbols->getLocals(f)) {
      auto lT = getInferredType(l);
      if (skip) {
        skip = false;
        s << "  " << l->getName() << " : " << *lT;
        continue;
      }
      s << ",\n  " + l->getName() << " : " << *lT;
      s << std::flush;
    }
    s << "\n}\n";
  }
}
