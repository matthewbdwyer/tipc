#include "TypeInference.h"
#include "TypeConstraint.h"
#include "TypeConstraintCollectVisitor.h"
#include "Unifier.h"
#include <memory>

/*
 * This implementation collects the constraints and then solves them with a
 * unifier instance.  The unifier then records the inferred type results that
 * can be subsequently queried.
 */
std::unique_ptr<TypeInference> TypeInference::check(ASTProgram* ast, SymbolTable* symbols) {
  TypeConstraintCollectVisitor visitor(symbols);
  ast->accept(&visitor);

  auto unifier =  std::make_unique<Unifier>(visitor.getCollectedConstraints());
  unifier->solve();
 
  return std::make_unique<TypeInference>(symbols, std::move(unifier));
}

std::shared_ptr<TipType> TypeInference::getInferredType(ASTDeclNode *node) {
  auto var = std::make_shared<TipVar>(node);
  return unifier->inferred(var);
};

void TypeInference::print(std::ostream &s) {
  s << "Functions : {"; 
  auto skip = true;
  for (auto f : symbols->getFunctions()) {
    if (skip) {
      skip = false;
      s << f->getName() << " : " << *getInferredType(f);
      continue;
    }
    s << ", " + f->getName() << " : " << *getInferredType(f); 
  }
  s << "}\n";

  for (auto f : symbols->getFunctions()) {
    s << "Locals for function " + f->getName() + " : {";
    skip = true;
    for (auto l : symbols->getLocals(f)) {
      auto lT = getInferredType(l);
      if (skip) {
        skip = false;
        s << l->getName() << " : " << *lT;
        continue;
      }
      s << ", " + l->getName() << " : " << *lT;
      s << std::flush;
    }
    s << "}\n";
  }
}

