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
    if (!solver->existNode(print())) {
        throw new TIPTypeError("Variable " + print() + " not declared");
    }
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

void InputExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(print(), new TIPint());
}

void AllocExpr::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
    solver->setType(print(), new TIPref(solver->getType(ARG->print())));
}

void RefExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(print(), new TIPref(solver->getType(NAME)));
}

void DeRefExpr::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
    TIPref* ref = dynamic_cast<TIPref*>(solver->getType(ARG.get()->print()));
    if (ref == nullptr) {
        throw TIPTypeError(ARG->print()+" cannot be dereferenced");
    }
    solver->setType(print(), ref->of);
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
    for (std::string var : VARS) {
        solver->addNode(var);
    }
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
    //else could be null
    if (ELSE != nullptr) {
        ELSE->typecheck(solver);
    }
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
    ARG->typecheck(solver);
}

void FunAppExpr::typecheck(UnionFindSolver* solver)
{
    //check function definition
    if (!solver->hasFun(FUN->print())) {
        throw TIPTypeError("Function "+FUN->print()+" not defined");
    }
    //check parameter count
    std::vector<std::string> params = solver->getFunParams(FUN->print());
    if (params.size() != ACTUALS.size()) {
        throw TIPTypeError("Function "+FUN->print()+" parameter number mismatch");
    }
    //unify parameters with definition
    for (int i = 0; i < params.size(); i++)
    {
        solver->unifyNodes(params[i], ACTUALS[i]->print());
    }
    //check return type
    solver->unifyNodes(print(), solver->getFunRet(FUN->print()));
}

void Function::definition(UnionFindSolver* solver)
{
    std::string ret = "";
    for (auto const &stmt : BODY) {
        if (stmt->print().substr(0,6) == "return") {
            ret = dynamic_cast<ReturnStmt*>(stmt.get())->printArg();
        }
    }
    if (ret == "") {
        throw TIPTypeError("No return statement found for function " + NAME);
    }
    solver->addFun(NAME, FORMALS, ret);
}

void Function::typecheck(UnionFindSolver* solver)
{
    for (std::string param : FORMALS) {
        solver->addNode(param);
    }
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

std::string Function::printTyped(UnionFindSolver* solver) {
    //print function signature
    std::string typedFun = NAME + "(";
    for (auto it = FORMALS.begin(); it != FORMALS.end(); ++it) {
        typedFun += *it;
        if (std::next(it) != FORMALS.end()) {
            typedFun += ",";
        }
    }
    typedFun += "): ";
    //build function type string
    std::string fun_type = "(";
    for (auto it = FORMALS.begin(); it != FORMALS.end(); ++it) {
        fun_type += solver->getType(*it)->print();
        if (std::next(it) != FORMALS.end()) {
            fun_type += ",";
        }
    }
    fun_type += ") -> ";
    fun_type += solver->getType(solver->getFunRet(NAME))->print();
    typedFun += fun_type;

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

std::string Program::printTyped() {
    UnionFindSolver* solver = new UnionFindSolver();
    for (auto const &fun : FUNCTIONS) {
        try
        {
            fun->definition(solver);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    for (auto const &fun : FUNCTIONS) {
        fun->typecheck(solver);
    }
    std::string typedProgram;
    for (auto const &fun : FUNCTIONS) {
        typedProgram += fun->printTyped(solver) + "\n";
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