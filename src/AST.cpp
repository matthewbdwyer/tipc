#include "AST.h"
#include "ASTVisitor.h"

#include <algorithm>

using namespace AST;

/*
 * Convert a vector of unique ptrs to pointers.
 * This is used when the client does not need ownership, 
 * e.g., they wish only to read the pointer.
 */
template<typename T>
std::vector<T*> rawRefs(std::vector<std::unique_ptr<T>> &v) {
  std::vector<T*> r;
  std::transform(v.begin(), v.end(), std::back_inserter(r), [](auto& up){return up.get();});
  return r;
}

std::vector<Expr*> FunAppExpr::getActuals() {
  return rawRefs(ACTUALS);
}

std::vector<FieldExpr*> RecordExpr::getFields() {
  return rawRefs(FIELDS);
}

std::vector<DeclNode*> DeclStmt::getVars() {
  return rawRefs(VARS);
}

std::vector<Stmt*> BlockStmt::getStmts() {
  return rawRefs(STMTS);
}

std::vector<DeclNode*> Function::getFormals() {
  return rawRefs(FORMALS);
}

std::vector<DeclStmt*> Function::getDeclarations() {
  return rawRefs(DECLS);
}

std::vector<Stmt*> Function::getStmts() {
  return rawRefs(BODY);
}

std::vector<Function*> Program::getFunctions() {
  return rawRefs(FUNCTIONS);
}

Function * Program::findFunctionByName(std::string name) {
    for(auto fn : getFunctions()) {
        if(fn->getName() == name) {
            return fn;
        }
    }
    return nullptr;
}

/*
 * Accept methods are defined for each subtype of node.  Their job is to 
 * determine if the visitor wishes to visit the substructure of the node,
 * based on the "visit()" call, and if so to call the "accept()" method of
 * the elements of the substructure in the order in which they are declared.  
 * The final responsibility is to call "endVisit()" which allows post-order 
 * processing of the nodes by a visitor. 
 */ 

void DeclNode::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

void NumberExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

void VariableExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

void BinaryExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
  }
  visitor->endVisit(this);
}

void FunAppExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getFunction()->accept(visitor);
    for (auto a : getActuals()) {
      a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

void InputExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

void AllocExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
  }
  visitor->endVisit(this);
}

void RefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getVar()->accept(visitor);
  }
  visitor->endVisit(this);
}

void DeRefExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getPtr()->accept(visitor);
  }
  visitor->endVisit(this);
}

void NullExpr::accept(ASTVisitor * visitor) {
  visitor->endVisit(this);
}

void FieldExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
  }
  visitor->endVisit(this);
}

void RecordExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto f : getFields()) {
      f->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

void AccessExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getRecord()->accept(visitor);
  }
  visitor->endVisit(this);
}

void DeclStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto v : getVars()) {
      v->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

void BlockStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

void AssignStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLHS()->accept(visitor);
    getRHS()->accept(visitor);
  }
  visitor->endVisit(this);
}

void WhileStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCondition()->accept(visitor);
    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}

void IfStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCondition()->accept(visitor);
    getThen()->accept(visitor);
    if (getElse() != nullptr) {
      getElse()->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

void OutputStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

void ErrorStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

void ReturnStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

void Function::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getDecl()->accept(visitor);
    for (auto p : getFormals()) {
      p->accept(visitor);
    }
    for (auto d : getDeclarations()) {
      d->accept(visitor);
    }
    for (auto s : getStmts()) {
      s->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

void Program::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto f : getFunctions()) {
      f->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
