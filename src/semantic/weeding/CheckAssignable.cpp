#include "CheckAssignable.h"
#include "PrettyPrinter.h"
#include "SemanticError.h"
#include <sstream>

void CheckAssignable::endVisit(ASTAssignStmt* element) {
  // These are the l-value expressions
  if (dynamic_cast<ASTVariableExpr*>(element->getLHS())) return; 
  if (dynamic_cast<ASTDeRefExpr*>(element->getLHS())) return;
  if (dynamic_cast<ASTAccessExpr*>(element->getLHS())) return;

  std::ostringstream oss;
  oss << "Assignment error on line " << element->getLine() << ": ";
  oss << element->getLHS() << " not an l-value\n";

  throw SemanticError(oss.str());
}

void CheckAssignable::check(ASTProgram* p) {
  CheckAssignable visitor;
  p->accept(&visitor);
}
