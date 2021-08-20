#pragma once

#include "ASTVisitor.h"
#include "Unifier.h"

/*! \class AbsentFieldChecker
 *  \brief Visits AST and checks that all field accesses are to defined fields
 *
 * This visitor performs a post-order traversal to analyze field accesses
 * either on the LHS or RHS of an assignment.  It uses the computed type
 * information to determine whether the access is to a field that has
 * been defined by some record expression in the program.  This is a flow
 * insensitive check.
 *
 * Errors are reported by throwing SemanticError exceptions.
 * \sa SemanticError
 */
class AbsentFieldChecker : public ASTVisitor {
  Unifier* unifier;
public:
  AbsentFieldChecker(Unifier* u) : unifier(u) {} 

  /*! \fn check
   *  \brief Generate and check absent field constraints and report any errors.
   *
   * \sa Semantic Error  
   * \param p The Program AST
   * \param u The unifier storing the computed type judgements
   */
  static void check(ASTProgram* p, Unifier* u);

  void endVisit(ASTAccessExpr * element) override;
};

