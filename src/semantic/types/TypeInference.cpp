#include "TypeInference.h"
#include "TypeConstraintVisitor.h"
#include "Unifier.h"
#include "TypeConstraint.h"
#include <memory>

namespace {

std::vector<TypeConstraint> collected;

void collectConstraints(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    collected.push_back(TypeConstraint(t1,t2));
}

};

void TypeInference::inferandcheck(ASTProgram* ast, SymbolTable* symbols) {
  TypeConstraintVisitor visitor(symbols, collectConstraints);
  ast->accept(&visitor);
  Unifier unifier(collected);
  unifier.solve();
}
