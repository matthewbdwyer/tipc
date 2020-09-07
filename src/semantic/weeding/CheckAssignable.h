#pragma once

#include "ASTVisitor.h"

/*! \class CheckAssignable
 *  \brief Check if left hand side of assignment is an l-value.
 *
 * The left-hand side of an assignment statement must be able to express an l-value
 * (i.e., an address).  In TIP the only expressions that can be l-values are:
 * \sa VariableExpr 
 * \sa DeRefExpr
 * \sa AccessExpr
 *
 * Assignability is also required when using the "&E" expression.  We can only take the
 * address of an expression if it has an l-value.
 *
 * This weeding pass checks where l-value expressions are required and throws a SemanticError otherwise.
 */
class CheckAssignable :  public ASTVisitor {
public:
    CheckAssignable() = default;
    static void check(ASTProgram* p);
    virtual void endVisit(ASTAssignStmt * element) override;
    virtual void endVisit(ASTRefExpr * element) override;
};

