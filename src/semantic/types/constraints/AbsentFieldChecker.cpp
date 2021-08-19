#include "AbsentFieldChecker.h"
#include "TipVar.h"
#include "TipAbsentField.h"
#include "SemanticError.h"

#include <sstream>

void AbsentFieldChecker::check(ASTProgram* p, Unifier* u) 
{
  AbsentFieldChecker visitor(u);
  p->accept(&visitor);
}

/*! \brief Check that the type of the field access is not absent 
 *
 * This check is a bit less nuanced than in the TIP scala implementation.
 * We simply report absent field accesses and do distinguish reads from writes.
 */
void AbsentFieldChecker::endVisit(ASTAccessExpr * element) {
  // Generate a new type variable for the access expression
  auto typeVar = std::make_shared<TipVar>(element);

  // Look up the inferred type for this variable in the type judgements
  auto inferredType = unifier->inferred(typeVar);

  // If the inferred type is an absent field, exit with an error message
  if (std::dynamic_pointer_cast<TipAbsentField>(inferredType) != nullptr) {
    std::stringstream sstream; 
    sstream << element;
    throw SemanticError("Access to absent field on line " + 
          std::to_string(element->getLine()) + " in column " + 
          std::to_string(element->getColumn()) + ": " + 
          sstream.str());
  }

}

