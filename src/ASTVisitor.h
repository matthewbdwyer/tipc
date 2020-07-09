#pragma once

#include "AST.h"

class ASTVisitor {
public:
  virtual bool visit(AST::Program * element) { return true; }
  virtual void endVisit(AST::Program * element) {}
  virtual bool visit(AST::Function * element) { return true; }
  virtual void endVisit(AST::Function * element) {}
  virtual bool visit(AST::NumberExpr * element) { return true; }
  virtual void endVisit(AST::NumberExpr * element) {}
  virtual bool visit(AST::VariableExpr * element) { return true; }
  virtual void endVisit(AST::VariableExpr * element) {}
  virtual bool visit(AST::BinaryExpr * element) { return true; }
  virtual void endVisit(AST::BinaryExpr * element) {}
  virtual bool visit(AST::InputExpr * element) { return true; }
  virtual void endVisit(AST::InputExpr * element) {}
  virtual bool visit(AST::FunAppExpr * element) { return true; }
  virtual void endVisit(AST::FunAppExpr * element) {}
  virtual bool visit(AST::AllocExpr * element) { return true; }
  virtual void endVisit(AST::AllocExpr * element) {}
  virtual bool visit(AST::RefExpr * element) { return true; }
  virtual void endVisit(AST::RefExpr * element) {}
  virtual bool visit(AST::DeRefExpr * element) { return true; }
  virtual void endVisit(AST::DeRefExpr * element) {}
  virtual bool visit(AST::NullExpr * element) { return true; }
  virtual void endVisit(AST::NullExpr * element) {}
  virtual bool visit(AST::FieldExpr * element) { return true; }
  virtual void endVisit(AST::FieldExpr * element) {}
  virtual bool visit(AST::RecordExpr * element) { return true; }
  virtual void endVisit(AST::RecordExpr * element) {}
  virtual bool visit(AST::AccessExpr * element) { return true; }
  virtual void endVisit(AST::AccessExpr * element) {}
  virtual bool visit(AST::DeclNode * element) { return true; }
  virtual void endVisit(AST::DeclNode * element) {}
  virtual bool visit(AST::DeclStmt * element) { return true; }
  virtual void endVisit(AST::DeclStmt * element) {}
  virtual bool visit(AST::AssignStmt * element) { return true; }
  virtual void endVisit(AST::AssignStmt * element) {}
  virtual bool visit(AST::WhileStmt * element) { return true; }
  virtual void endVisit(AST::WhileStmt * element) {}
  virtual bool visit(AST::IfStmt * element) { return true; }
  virtual void endVisit(AST::IfStmt * element) {}
  virtual bool visit(AST::OutputStmt * element) { return true; }
  virtual void endVisit(AST::OutputStmt * element) {}
  virtual bool visit(AST::ReturnStmt * element) { return true; }
  virtual void endVisit(AST::ReturnStmt * element) {}
  virtual bool visit(AST::ErrorStmt * element) { return true; }
  virtual void endVisit(AST::ErrorStmt * element) {}
  virtual bool visit(AST::BlockStmt * element) { return true; }
  virtual void endVisit(AST::BlockStmt * element) {}
};

