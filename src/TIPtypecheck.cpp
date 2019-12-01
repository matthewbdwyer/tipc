#include "TIPtree.h"
#include "UnionFindSolver.h"
#include "TIPtypes.h"
#include <iostream>

namespace TIPtree {

void NumberExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(print(), new TIPint());
}

void VariableExpr::typecheck(UnionFindSolver* solver)
{
    solver->addNode(print());
}

void BinaryExpr::typecheck(UnionFindSolver* solver)
{
    LHS->typecheck(solver);
    RHS->typecheck(solver);
    if (OP == "=") {
        solver->unifyNodes(LHS->print(), RHS->print());
    } else {
        solver->setType(LHS->print(), new TIPint());
        solver->setType(RHS->print(), new TIPint());
    }
    solver->setType(print(), new TIPint());
}

void FunAppExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void InputExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(print(), new TIPint());
}

void AllocExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void RefExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(print(), new TIPref(new TIPint()));
}

void DeRefExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void NullExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void FieldExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void RecordExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void AccessExpr::typecheck(UnionFindSolver* solver)
{
    return;
}

void DeclStmt::typecheck(UnionFindSolver* solver)
{
    return;
}

void BlockStmt::typecheck(UnionFindSolver* solver)
{
    for (auto const &stmt : STMTS) {
        stmt->typecheck(solver);
    }
}

void AssignStmt::typecheck(UnionFindSolver* solver)
{
    LHS->typecheck(solver);
    RHS->typecheck(solver);
    solver->unifyNodes(LHS->print(), RHS->print());
}

void WhileStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "WhileStmt" << std::endl;
    COND->typecheck(solver);
    solver->setType(COND->print(), new TIPint());
    BODY->typecheck(solver);
}

void IfStmt::typecheck(UnionFindSolver* solver)
{
    COND->typecheck(solver);
    solver->setType(COND->print(), new TIPint());
    THEN->typecheck(solver);
    ELSE->typecheck(solver);
}

void OutputStmt::typecheck(UnionFindSolver* solver)
{
    solver->setType(ARG->print(), new TIPint());
}

void ErrorStmt::typecheck(UnionFindSolver* solver)
{
    return;
}

void ReturnStmt::typecheck(UnionFindSolver* solver)
{
    return;
}

void Function::typecheck(UnionFindSolver* solver)
{
    for (auto const &decl : DECLS) {
        try
        {
            decl->typecheck(solver);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Exception at: " << decl->print() << e.what() << '\n';
            throw e;
        }
    }
    for (auto const &stmt : BODY) {
        try
        {
            stmt->typecheck(solver);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Exception at: " << stmt->print() << e.what() << '\n';
            throw e;
        }
    }
}

std::string Function::printTyped() {
    UnionFindSolver* solver = new UnionFindSolver();
    typecheck(solver);
    //print function signature
    std::string typedFun = NAME + "(";
    for (auto it = FORMALS.begin(); it != FORMALS.end(); ++it) {
        typedFun += *it;
        if (std::next(it) != FORMALS.end()) {
            typedFun += ",";
        }
    }
    typedFun += "): ";
    //print function type
    //TODO: implementation
    typedFun += "function type";

    typedFun += "\n{\n";
    for (auto const &decl : DECLS) {
        typedFun += decl->printTyped(solver) + "\n";
    }
    for (auto const &stmt : BODY) {
        typedFun += stmt->print() + "\n";
    }
    typedFun += "}\n";
    return typedFun;
}

void Program::typecheck()
{
    for (auto const &fun : FUNCTIONS) {
        fun->typecheck(new UnionFindSolver());
    }
}

std::string Program::printTyped() {
    std::string typedProgram;
    for (auto const &fun : FUNCTIONS) {
        typedProgram += fun->printTyped() + "\n";
    }
    return typedProgram;
}

std::string DeclStmt::printTyped(UnionFindSolver* solver) {
    std::string declTyped = "var ";
    // comma separated variable names list
    for (auto it = VARS.begin(); it != VARS.end(); ++it) {
        declTyped += *it + ": ";
        declTyped += solver->getType(*it)->print();
        if (std::next(it) != VARS.end()) {
            declTyped += ",";
        }
    }
    declTyped += ";";
    return declTyped;
}
}