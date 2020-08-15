#include "TypeInference.h"
#include "TypeConstraintVisitor.h"
#include "TypeConstraintUnifyVisitor.h"
#include "Unifier.h"
#include "TypeConstraint.h"
#include <memory>


void TypeInference::inferandcheck(ASTProgram* ast, SymbolTable* symbols) {
  TypeConstraintUnifyVisitor visitor(symbols);
  ast->accept(&visitor);
}
