#include "ASTVisitor.h"
#include "AST.h"

void AST::NumberExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::VariableExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::BinaryExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::FunAppExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::InputExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::AllocExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::RefExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::DeRefExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::NullExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::FieldExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::RecordExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::AccessExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::DeclStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::BlockStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::AssignStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::WhileStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::IfStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::OutputStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::ErrorStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::ReturnStmt::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::Function::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}

void AST::Program::accept(ASTVisitor * visitor) {
  visitor->visit(this);
}
