#include "TIPtree.h"
#include "UnionFindSolver.h"
#include "TIPtypes.h"
#include <iostream>

namespace TIPtree {

void NumberExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(getId(), new TIPint());
}

void VariableExpr::typecheck(UnionFindSolver* solver)
{
    if (!solver->existNode(getId())) {
        throw TIPTypeError("Variable " + print() + " not declared");
    }
}

void BinaryExpr::typecheck(UnionFindSolver* solver)
{
    LHS->typecheck(solver);
    RHS->typecheck(solver);
    if (OP == "=") {
        solver->unifyNodes(LHS->getId(), RHS->getId());
    } else {
        solver->setType(LHS->getId(), new TIPint());
        solver->setType(RHS->getId(), new TIPint());
    }
    solver->setType(getId(), new TIPint());
}

void InputExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(getId(), new TIPint());
}

void AllocExpr::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
    solver->setType(getId(), new TIPref(solver->getType(ARG->getId())));
}

void RefExpr::typecheck(UnionFindSolver* solver)
{
    solver->setType(getId(), new TIPref(solver->getType(refId)));
}

void DeRefExpr::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
    TIPref* ref = dynamic_cast<TIPref*>(solver->getType(ARG->getId()));
    if (ref == nullptr) {
        throw TIPTypeError(ARG->print()+" cannot be dereferenced");
    }
    solver->setType(getId(), ref->of);
}

void DeclStmt::typecheck(UnionFindSolver* solver)
{
    for (int var_id : VAR_IDS) {
        solver->addNode(var_id);
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
    std::string left = LHS->print();
    std::string right = RHS->print();
    std::string left_type = solver->getType(LHS->getId())->print();
    std::string right_type = solver->getType(RHS->getId())->print();
    solver->unifyNodes(LHS->getId(), RHS->getId());
}

void WhileStmt::typecheck(UnionFindSolver* solver)
{
    COND->typecheck(solver);
    solver->setType(COND->getId(), new TIPint());
    BODY->typecheck(solver);
}

void IfStmt::typecheck(UnionFindSolver* solver)
{
    COND->typecheck(solver);
    solver->setType(COND->getId(), new TIPint());
    THEN->typecheck(solver);
    //else could be null
    if (ELSE != nullptr) {
        ELSE->typecheck(solver);
    }
}

void OutputStmt::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
    solver->setType(ARG->getId(), new TIPint());
}

void ReturnStmt::typecheck(UnionFindSolver* solver)
{
    ARG->typecheck(solver);
}

void FunAppExpr::typecheck(UnionFindSolver* solver)
{
    TIPfun* funType = dynamic_cast<TIPfun*>(solver->getType(FUN->getId()));
    if (funType == nullptr) {
        solver->addNode(FUN->getId());
        std::vector<TIPtype*> param_types;
        for (auto const& param : ACTUALS) {
            param_types.push_back(solver->getType(param->getId()));
        }
        solver->setType(FUN->getId(), new TIPfun(param_types, new TIPalpha()));
        return;
    }
    std::vector<TIPtype*> param_types = funType->param_types;
    if (param_types.size() != ACTUALS.size()) {
        throw TIPTypeError("calling function "+FUN->print()+" with invalid number of parameters");
    }
    for (int i = 0; i < param_types.size(); i++) {
        solver->setType(ACTUALS[i]->getId(), param_types[i]);
    }
    solver->setType(getId(), funType->ret);
    //unify funappexpr type information with fun definition
    std::vector<TIPtype*> updated_param_types;
    for (auto const& actual : ACTUALS) {
        updated_param_types.push_back(solver->getType(actual->getId()));
    }
    TIPtype* updated_ret_type = solver->getType(getId());
    solver->setType(FUN->getId(), new TIPfun(updated_param_types, updated_ret_type));
}



void Function::typecheck(UnionFindSolver* solver)
{
    for (int param_id : FORMAL_IDS) {
        solver->addNode(param_id);
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
            ret_type = solver->getType(dynamic_cast<ReturnStmt*>(stmt.get())->getArgId());
            break;
        }
    }
    if (ret_type == nullptr) {
        throw TIPTypeError("No return statement found for function " + NAME);
    }
    std::vector<TIPtype*> param_types;
    for (int param : FORMAL_IDS) {
        param_types.push_back(solver->getType(param));
    }
    solver->addNode(getId());
    solver->setType(getId(), new TIPfun(param_types, ret_type));
    //unify params with function type info
    TIPfun* fun_type = dynamic_cast<TIPfun*>(solver->getType(getId()));
    for (int i=0;i<FORMAL_IDS.size();i++) {
        solver->setType(FORMAL_IDS[i], fun_type->param_types[i]);
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
    typedFun += solver->getType(getId())->print();
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
    for (int i=0;i<3;i++) {
        for (auto const &fun : FUNCTIONS) {
            fun->typecheck(solver);
        }
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
    for (int i=0; i<VARS.size(); i++) {
        declTyped += VARS[i] + ": ";
        declTyped += solver->getType(VAR_IDS[i])->print();
        if (i != VARS.size()-1) {
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