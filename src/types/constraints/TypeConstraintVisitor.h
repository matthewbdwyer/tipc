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

    void endVisit(AST::Program * element) override;
    bool visit(AST::Function * element) override;
    void endVisit(AST::Function * element) override;
    void endVisit(AST::NumberExpr * element) override;
    void endVisit(AST::VariableExpr * element) override;
    void endVisit(AST::BinaryExpr * element) override;
    void endVisit(AST::InputExpr * element) override;
    void endVisit(AST::FunAppExpr * element) override;
    void endVisit(AST::AllocExpr * element) override;
    void endVisit(AST::RefExpr * element) override;
    void endVisit(AST::DeRefExpr * element) override;
    void endVisit(AST::NullExpr * element) override;
    void endVisit(AST::FieldExpr * element) override;
    void endVisit(AST::RecordExpr * element) override;
    void endVisit(AST::AccessExpr * element) override;
    void endVisit(AST::DeclNode * element) override;
    void endVisit(AST::DeclStmt * element) override;
    void endVisit(AST::AssignStmt * element) override;
    void endVisit(AST::WhileStmt * element) override;
    void endVisit(AST::IfStmt * element) override;
    void endVisit(AST::OutputStmt * element) override;
    void endVisit(AST::ReturnStmt * element) override;
    void endVisit(AST::ErrorStmt * element) override;
    void endVisit(AST::BlockStmt * element) override;

    std::vector<TypeConstraint> get_constraints();
private:
    std::vector<TypeConstraint> constraints;
    SymbolTable &symbolTable;
    std::set<std::string> &appearingFields;

    // NB: Each node pushes itself onto the visitResults stack. This allows
    // the canonical representations of identifiers to propagate up to
    // where they are used.
    std::stack<std::shared_ptr<TipType>> visitResults;
    std::stack<AST::DeclNode *> scope;
};

