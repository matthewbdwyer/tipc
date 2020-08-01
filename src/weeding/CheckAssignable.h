#pragma once

#include "ASTVisitor.h"
#include <iostream>

/*
 * The left-hand side of an assignment statement must be able to express an l-value
 * (i.e., an address).  In TIP the only expressions that can be l-values are:
 *    VariableExpr, DeRefExpr, or AccessExpr
 *
 * This weeding pass checks that assignment statements are to l-value expressions
 * and prints an error otherwise.  The "check" method returns whether there is an
 * error or not.
 */
class CheckAssignable :  public ASTVisitor {
public:
    CheckAssignable() = default;
    std::string errorString;
    static bool check(ASTProgram* p, std::ostream &os);
    virtual void endVisit(ASTAssignStmt * element) override;
};

