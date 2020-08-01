#include "CheckAssignable.h"
#include "PrettyPrinter.h"
#include <sstream>

void CheckAssignable::endVisit(ASTAssignStmt* element) {
  // These are the l-value expressions
  if (dynamic_cast<ASTVariableExpr*>(element->getLHS())) return; 
  if (dynamic_cast<ASTDeRefExpr*>(element->getLHS())) return;
  if (dynamic_cast<ASTAccessExpr*>(element->getLHS())) return;

  std::stringstream errorStream;
  PrettyPrinter pp(errorStream, ' ', 2);
  pp.print2(element->getLHS(), errorStream, ' ', 2);

  errorString += "Assignment Error: expression " + errorStream.str() + " not an l-value\n";
}

bool CheckAssignable::check(ASTProgram* p, std::ostream &os) {
  CheckAssignable visitor;
  p->accept(&visitor);

  // Return check result based on whether an error has been recorded
  bool errorFound = (visitor.errorString == "");
  if (errorFound) {
    os << visitor.errorString;
  }
  return errorFound;
}
