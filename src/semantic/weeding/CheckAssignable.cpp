#include "CheckAssignable.h"
#include "PrettyPrinter.h"
#include "SemanticError.h"
#include <sstream>

void CheckAssignable::endVisit(ASTAssignStmt* element) {
  // These are the l-value expressions
  if (dynamic_cast<ASTVariableExpr*>(element->getLHS())) return; 
  if (dynamic_cast<ASTDeRefExpr*>(element->getLHS())) return;
  if (dynamic_cast<ASTAccessExpr*>(element->getLHS())) return;

  std::stringstream errorStream;
  PrettyPrinter::print2(element->getLHS(), errorStream, ' ', 2);

  throw SemanticError("Assignment error: " + errorStream.str() + " not an l-value\n");
}

void CheckAssignable::check(ASTProgram* p) {
  CheckAssignable visitor;
  p->accept(&visitor);
}
