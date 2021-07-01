#include "CheckAssignable.h"
#include "PrettyPrinter.h"
#include "SemanticError.h"

#include <sstream>

namespace {

// Return true if expression has an l-value
bool isAssignable(ASTExpr* e) {
  if (dynamic_cast<ASTVariableExpr*>(e)) return true;
  if (dynamic_cast<ASTAccessExpr*>(e)) return true;
  return false;
}

}

void CheckAssignable::endVisit(ASTAssignStmt* element) {
  if (isAssignable(element->getLHS())) return;

  // Assigning through a pointer is also permitted
  if (dynamic_cast<ASTDeRefExpr*>(element->getLHS())) return;

  std::ostringstream oss;
  oss << "Assignment error on line " << element->getLine() << ": ";
  oss << *element->getLHS() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::endVisit(ASTRefExpr* element) {
  if (isAssignable(element->getVar())) return;

  std::ostringstream oss;
  oss << "Address of error on line " << element->getLine() << ": ";
  oss << *element->getVar() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::check(ASTProgram* p) {
  CheckAssignable visitor;
  p->accept(&visitor);
}
