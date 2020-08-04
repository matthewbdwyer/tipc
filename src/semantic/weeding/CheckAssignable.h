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
 * This weeding pass checks that assignment statements are to l-value expressions
 * and throws a SemanticError otherwise.
 */
class CheckAssignable :  public ASTVisitor {
public:
    CheckAssignable() = default;
    static void check(ASTProgram* p);
    virtual void endVisit(ASTAssignStmt * element) override;
};

