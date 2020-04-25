#include "TIPtreeVisitor.h"
#include "TIPtree.h"

void TIPtree::NumberExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::VariableExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::BinaryExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::FunAppExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::InputExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::AllocExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::RefExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::DeRefExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::NullExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::FieldExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::RecordExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::AccessExpr::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::DeclStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::BlockStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::AssignStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::WhileStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::IfStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::OutputStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::ErrorStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::ReturnStmt::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::Function::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}

void TIPtree::Program::accept(TIPtreeVisitor * visitor) {
  visitor->visit(this);
}
