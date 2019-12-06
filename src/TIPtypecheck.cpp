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

void ReturnStmt::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
}

void FunAppExpr::typecheck(UnionFindSolver* solver)
{
    TIPfun* funType = dynamic_cast<TIPfun*>(solver->getType(FUN->print()));
    if (funType == nullptr) {
        solver->addNode(FUN->print());
        std::vector<TIPtype*> params_type;
        for (auto const& param : ACTUALS) {
            params_type.push_back(solver->getType(param->print()));
        }
        solver->setType(FUN->print(), new TIPfun(params_type, new TIPalpha()));
        return;
    }
    std::vector<TIPtype*> params_type = funType->params_type;
    if (params_type.size() != ACTUALS.size()) {
        throw TIPTypeError("calling function "+FUN->print()+" with invalid number of parameters");
    }
    for (int i = 0; i < params_type.size(); i++) {
        solver->setType(ACTUALS[i]->print(), params_type[i]);
    }
    solver->setType(print(), funType->ret);
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
    TIPtype* ret_type = nullptr;
    for (auto const &stmt : BODY) {
        if (stmt->print().substr(0,6) == "return") {
            ret_type = solver->getType(dynamic_cast<ReturnStmt*>(stmt.get())->printArg());
            break;
        }
    }
    if (ret_type == nullptr) {
        throw TIPTypeError("No return statement found for function " + NAME);
    }
    std::vector<TIPtype*> params_type;
    for (std::string param : FORMALS) {
        params_type.push_back(solver->getType(param));
    }
    solver->addNode(NAME);
    solver->setType(NAME, new TIPfun(params_type, ret_type));
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
    typedFun += solver->getType(NAME)->print();
    typedFun += "\n{\n";
    for (auto const &decl : DECLS) {
        typedFun += decl->printTyped(solver) + "\n";
    }
    for (auto const &stmt : BODY) {
        typedFun += stmt->print() + "\n";
    }
    typedFun += "}";
    return typedFun;
}

std::string Program::printTyped() {
    UnionFindSolver* solver = new UnionFindSolver();
    //typecheck functions twice to check function use before function definition
    for (auto const &fun : FUNCTIONS) {
        fun->typecheck(solver);
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

void Program::typecheck(UnionFindSolver* solver) {
    return;
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

/**
 * Empty typechecks: record is not implemented, nullexpr and errorstmt 
 * do not need typechecking  
 */
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

void ErrorStmt::typecheck(UnionFindSolver* solver)
{
    return;
}

}