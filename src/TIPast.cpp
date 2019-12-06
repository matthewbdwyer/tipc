#include "TIPtree.h"
#include "TIPtypes.h"

namespace TIPtree {

static int ast_counter = 1;
// mapping from function name to astnode id
static std::map<std::string, int> fun2id;
// mapping from variable name to astnode id
static std::map<std::string, int> var2id;

int AstNode::getId() {
    return this->id;
}

void NumberExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void VariableExpr::genId() {
    if (this->id) {
        return;
    }
    if (var2id.find(NAME) != var2id.end()) {
        this->id = var2id[NAME];
        return;
    }
    if (fun2id.find(NAME) != fun2id.end()) {
        this->id = fun2id[NAME];
        return;
    }
    throw TIPTypeError("Undefined variable reference: "+NAME);
}

void BinaryExpr::genId() {
    if (!this->id) {
        return;
    }
    this->id = ast_counter++;
    LHS->genId();
    RHS->genId();
}

void FunAppExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    //no need to gen id for fun ptr
    for (auto const& actual : ACTUALS) {
        actual->genId();
    }
}

void InputExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void AllocExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    ARG->genId();
}

void RefExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void DeRefExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    ARG->genId();
}

void NullExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void FieldExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void RecordExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void AccessExpr::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
}

void DeclStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    for (std::string var : VARS) {
        var2id[var] = ast_counter++;
    }
}

void BlockStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    for (auto const& stmt : STMTS) {
        stmt->genId();
    }
}

void AssignStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    LHS->genId();
    RHS->genId();
}

void WhileStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    COND->genId();
    BODY->genId();
}

void IfStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    COND->genId();
    THEN->genId();
    if (ELSE) {
        ELSE->genId();
    }
}

void OutputStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    ARG->genId();
}

void ErrorStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    ARG->genId();
}

void ReturnStmt::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    ARG->genId();
}

void Function::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    fun2id[NAME] = this->id;
    //clear variable definitions
    var2id.clear();
    for (std::string param : FORMALS) {
        var2id[param] = ast_counter++;
    }
    for (auto const &decl : DECLS) {
        decl->genId();
    }
    for (auto const &stmt : BODY) {
        stmt->genId();
    }
}

void Program::genId() {
    if (this->id) {
        return;
    }
    this->id = ast_counter++;
    for (auto const& fun : FUNCTIONS) {
        fun->genId();
    }
}
}
