#pragma once

#include "AST.h"

/*! \brief Base class for AST visitors.
 *
 * The AST visitor class abstracts the traversal of an AST.  It works
 * in concert with the accept methods of ASTNode subtypes.  Each of those
 * nodes performs a traversal of its children in an order corresponding to
 * their appearance in the source program.  This class defines default behavior
 * for the processing performed when the traversal reaches a node of a given type.
 * By default the visit method returns true, indicating that the children of
 * the node should also be visited, and the endVisit method does nothing.
 *
 * Subtypes of ASTVisitor will selectively override these default methods.
 * Overriding a visit permits a pre-order processing during traversal and
 * overriding endVisit permits a post-order processing.
 *
 * Subtype of ASTVisitor will also define member data that can be referenced
 * by overridden methods to communicate information along the traversal to
 * future method invocations.
 */
class ASTVisitor {
public:
  virtual bool visit(ASTProgram * element) { return true; }
  virtual void endVisit(ASTProgram * element) {}
  virtual bool visit(ASTFunction * element) { return true; }
  virtual void endVisit(ASTFunction * element) {}
  virtual bool visit(ASTNumberExpr * element) { return true; }
  virtual void endVisit(ASTNumberExpr * element) {}
  virtual bool visit(ASTVariableExpr * element) { return true; }
  virtual void endVisit(ASTVariableExpr * element) {}
  virtual bool visit(ASTBinaryExpr * element) { return true; }
  virtual void endVisit(ASTBinaryExpr * element) {}
  virtual bool visit(ASTInputExpr * element) { return true; }
  virtual void endVisit(ASTInputExpr * element) {}
  virtual bool visit(ASTFunAppExpr * element) { return true; }
  virtual void endVisit(ASTFunAppExpr * element) {}
  virtual bool visit(ASTAllocExpr * element) { return true; }
  virtual void endVisit(ASTAllocExpr * element) {}
  virtual bool visit(ASTRefExpr * element) { return true; }
  virtual void endVisit(ASTRefExpr * element) {}
  virtual bool visit(ASTDeRefExpr * element) { return true; }
  virtual void endVisit(ASTDeRefExpr * element) {}
  virtual bool visit(ASTNullExpr * element) { return true; }
  virtual void endVisit(ASTNullExpr * element) {}
  virtual bool visit(ASTFieldExpr * element) { return true; }
  virtual void endVisit(ASTFieldExpr * element) {}
  virtual bool visit(ASTRecordExpr * element) { return true; }
  virtual void endVisit(ASTRecordExpr * element) {}
  virtual bool visit(ASTAccessExpr * element) { return true; }
  virtual void endVisit(ASTAccessExpr * element) {}
  virtual bool visit(ASTDeclNode * element) { return true; }
  virtual void endVisit(ASTDeclNode * element) {}
  virtual bool visit(ASTDeclStmt * element) { return true; }
  virtual void endVisit(ASTDeclStmt * element) {}
  virtual bool visit(ASTAssignStmt * element) { return true; }
  virtual void endVisit(ASTAssignStmt * element) {}
  virtual bool visit(ASTWhileStmt * element) { return true; }
  virtual void endVisit(ASTWhileStmt * element) {}
  virtual bool visit(ASTIfStmt * element) { return true; }
  virtual void endVisit(ASTIfStmt * element) {}
  virtual bool visit(ASTOutputStmt * element) { return true; }
  virtual void endVisit(ASTOutputStmt * element) {}
  virtual bool visit(ASTReturnStmt * element) { return true; }
  virtual void endVisit(ASTReturnStmt * element) {}
  virtual bool visit(ASTErrorStmt * element) { return true; }
  virtual void endVisit(ASTErrorStmt * element) {}
  virtual bool visit(ASTBlockStmt * element) { return true; }
  virtual void endVisit(ASTBlockStmt * element) {}
};

