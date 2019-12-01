#include "TIPtree.h"
#include "UnionFindSolver.h"
#include "TIPtypes.h"
#include <iostream>

namespace TIPtree {

void NumberExpr::typecheck(UnionFindSolver* solver)
{
    //std::cout << "NumberExpr" << std::endl;
    solver->setType(this, new TIPint());
}

void VariableExpr::typecheck(UnionFindSolver* solver)
{
    //std::cout << "VariableExpr" << std::endl;
    solver->addNode(this);
}

void BinaryExpr::typecheck(UnionFindSolver* solver)
{
    //std::cout << "BinaryExpr" << std::endl;
    LHS->typecheck(solver);
    RHS->typecheck(solver);
    if (OP == "=") {
        solver->unifyNodes(LHS.get(), RHS.get());
    } else {
        solver->setType(LHS.get(), new TIPint());
        solver->setType(RHS.get(), new TIPint());
    }
    solver->setType(this, new TIPint());
}

void FunAppExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "FunAppExpr" << std::endl;
    return;
}

void InputExpr::typecheck(UnionFindSolver* solver)
{
    //std::cout << "InputExpr" << std::endl;
    solver->setType(this, new TIPint());
}

void AllocExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "AllocExpr" << std::endl;
    return;
}

void RefExpr::typecheck(UnionFindSolver* solver)
{
    //std::cout << "RefExpr" << std::endl;
    solver->setType(this, new TIPref(new TIPint()));
}

void DeRefExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "DeRefExpr" << std::endl;
    return;
}

void NullExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "NullExpr" << std::endl;
    return;
}

void FieldExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "FieldExpr" << std::endl;
    return;
}

void RecordExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "RecordExpr" << std::endl;
    return;
}

void AccessExpr::typecheck(UnionFindSolver* solver)
{
    //TODO: implementation
    //std::cout << "AccessExpr" << std::endl;
    return;
}

void DeclStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "DeclStmt" << std::endl;
    return;
}

void BlockStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "BlockStmt" << std::endl;
    for (auto const &stmt : STMTS) {
        stmt->typecheck(solver);
    }
}

void AssignStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "AssignStmt" << std::endl;
    LHS->typecheck(solver);
    RHS->typecheck(solver);
    solver->unifyNodes(LHS.get(), RHS.get());
}

void WhileStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "WhileStmt" << std::endl;
    COND->typecheck(solver);
    solver->setType(COND.get(), new TIPint());
    BODY->typecheck(solver);
}

void IfStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "IfStmt" << std::endl;
    COND->typecheck(solver);
    solver->setType(COND.get(), new TIPint());
    THEN->typecheck(solver);
    ELSE->typecheck(solver);
}

void OutputStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "OutputStmt" << std::endl;
    solver->setType(ARG.get(), new TIPint());
}

void ErrorStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "ErrorStmt" << std::endl;
    return;
}

void ReturnStmt::typecheck(UnionFindSolver* solver)
{
    //std::cout << "ReturnStmt" << std::endl;
    return;
}

void Function::typecheck(UnionFindSolver* solver)
{
    //std::cout << "Function" << std::endl;
    for (auto const &decl : DECLS) {
        try
        {
            decl->typecheck(solver);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Exception at: " << decl->print() << e.what() << '\n';
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
        }
    }
}

void Program::typecheck()
{
    //std::cout << "Program" << std::endl;
    for (auto const &fun : FUNCTIONS) {
        fun->typecheck(new UnionFindSolver());
    }
}

}
