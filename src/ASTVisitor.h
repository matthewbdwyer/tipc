#pragma once

#include "AST.h"

class ASTVisitor {
public:
  virtual void visit(AST::Program * element)  = 0;
  virtual void visit(AST::Function * element) = 0;
  virtual void visit(AST::NumberExpr * element) = 0;
  virtual void visit(AST::VariableExpr * element) = 0;
  virtual void visit(AST::BinaryExpr * element) = 0;
  virtual void visit(AST::InputExpr * element) = 0;
  virtual void visit(AST::FunAppExpr * element) = 0;
  virtual void visit(AST::AllocExpr * element) = 0;
  virtual void visit(AST::RefExpr * element) = 0;
  virtual void visit(AST::DeRefExpr * element) = 0;
  virtual void visit(AST::NullExpr * element) = 0;
  virtual void visit(AST::FieldExpr * element) = 0;
  virtual void visit(AST::RecordExpr * element) = 0;
  virtual void visit(AST::AccessExpr * element) = 0;
  virtual void visit(AST::DeclStmt * element) = 0;
  virtual void visit(AST::AssignStmt * element) = 0;
  virtual void visit(AST::WhileStmt * element) = 0;
  virtual void visit(AST::IfStmt * element) = 0;
  virtual void visit(AST::OutputStmt * element) = 0;
  virtual void visit(AST::ReturnStmt * element) = 0;
  virtual void visit(AST::ErrorStmt * element) = 0;
  virtual void visit(AST::BlockStmt * element) = 0;
};

