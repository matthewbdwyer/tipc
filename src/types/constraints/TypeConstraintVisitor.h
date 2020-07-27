#pragma once

#include "SymbolTable.h"
#include "ASTVisitor.h"
#include "TipVar.h"
#include "TypeConstraint.h"
#include <stack>
#include <vector>
#include <string>
#include <set>

static const char *const ENTRYPOINT_NAME = "main";

class TypeConstraintVisitor: public ASTVisitor {
public:
    TypeConstraintVisitor() = delete;
    TypeConstraintVisitor(SymbolTable &table, std::set<std::string> &appearingFields);

    void endVisit(ASTProgram * element) override;
    bool visit(ASTFunction * element) override;
    void endVisit(ASTFunction * element) override;
    void endVisit(ASTNumberExpr * element) override;
    void endVisit(ASTVariableExpr * element) override;
    void endVisit(ASTBinaryExpr * element) override;
    void endVisit(ASTInputExpr * element) override;
    void endVisit(ASTFunAppExpr * element) override;
    void endVisit(ASTAllocExpr * element) override;
    void endVisit(ASTRefExpr * element) override;
    void endVisit(ASTDeRefExpr * element) override;
    void endVisit(ASTNullExpr * element) override;
    void endVisit(ASTFieldExpr * element) override;
    void endVisit(ASTRecordExpr * element) override;
    void endVisit(ASTAccessExpr * element) override;
    void endVisit(ASTDeclNode * element) override;
    void endVisit(ASTDeclStmt * element) override;
    void endVisit(ASTAssignStmt * element) override;
    void endVisit(ASTWhileStmt * element) override;
    void endVisit(ASTIfStmt * element) override;
    void endVisit(ASTOutputStmt * element) override;
    void endVisit(ASTReturnStmt * element) override;
    void endVisit(ASTErrorStmt * element) override;
    void endVisit(ASTBlockStmt * element) override;

    std::vector<TypeConstraint> get_constraints();
private:
    std::vector<TypeConstraint> constraints;
    SymbolTable &symbolTable;
    std::set<std::string> &appearingFields;

    // NB: Each node pushes itself onto the visitResults stack. This allows
    // the canonical representations of identifiers to propagate up to
    // where they are used.
    std::stack<std::shared_ptr<TipType>> visitResults;
    std::stack<ASTDeclNode *> scope;
};

