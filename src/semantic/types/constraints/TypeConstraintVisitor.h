#pragma once

#include "ASTVisitor.h"
#include "SymbolTable.h"
#include "TipType.h"
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <memory>

/*! \class TypeConstraintVisitor
 *  \brief Visitor generates type constraints and processes them.
 *
 * This visitor performs a post-order visit of the program AST.  It
 * constructs type constraints for each node that requires a type judgement.
 * The constraints are processed by a function that is passed as a parameter
 * to the visitor.  This provides flexibility in using the visitor - it
 * can simply record the constraints or it can solve them on the fly.
 */
class TypeConstraintVisitor: public ASTVisitor {
public:
    TypeConstraintVisitor() = delete;
    TypeConstraintVisitor(SymbolTable* t, void (*p)(std::shared_ptr<TipType>, 
                                                    std::shared_ptr<TipType>)) : 
            symbolTable(t), process(p) {};

    bool visit(ASTFunction * element) override;
    void endVisit(ASTAccessExpr * element) override;
    void endVisit(ASTAllocExpr * element) override;
    void endVisit(ASTAssignStmt * element) override;
    void endVisit(ASTBinaryExpr * element) override;
    void endVisit(ASTDeRefExpr * element) override;
    void endVisit(ASTErrorStmt * element) override;
    void endVisit(ASTFunAppExpr * element) override;
    void endVisit(ASTFunction * element) override;
    void endVisit(ASTIfStmt * element) override;
    void endVisit(ASTInputExpr * element) override;
    void endVisit(ASTNullExpr * element) override;
    void endVisit(ASTNumberExpr * element) override;
    void endVisit(ASTOutputStmt * element) override;
    void endVisit(ASTRecordExpr * element) override;
    void endVisit(ASTRefExpr * element) override;
    void endVisit(ASTWhileStmt * element) override;

private:
    void (*process)(std::shared_ptr<TipType>, std::shared_ptr<TipType>);
    std::stack<ASTDeclNode *> scope;
    SymbolTable *symbolTable;
    std::shared_ptr<TipType> astToVar(ASTNode * n);
};

