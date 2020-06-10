#pragma once

#include "ASTVisitor.h"

/**
 * Sample Visitor to illustrate traversing the AST.
 */
class SampleVisitor: public ASTVisitor {
public:
  void visit(AST::Program * element) override;
  void visit(AST::Function * element) override;
  void visit(AST::NumberExpr * element) override;
  void visit(AST::VariableExpr * element) override;
  void visit(AST::BinaryExpr * element) override;
  void visit(AST::InputExpr * element) override;
  void visit(AST::FunAppExpr * element) override;
  void visit(AST::AllocExpr * element) override;
  void visit(AST::RefExpr * element) override;
  void visit(AST::DeRefExpr * element) override;
  void visit(AST::NullExpr * element) override;
  void visit(AST::FieldExpr * element) override;
  void visit(AST::RecordExpr * element) override;
  void visit(AST::AccessExpr * element) override;
  void visit(AST::DeclStmt * element) override;
  void visit(AST::AssignStmt * element) override;
  void visit(AST::BlockStmt * element) override;
  void visit(AST::WhileStmt * element) override;
  void visit(AST::IfStmt * element) override;
  void visit(AST::OutputStmt * element) override;
  void visit(AST::ErrorStmt * element) override;
  void visit(AST::ReturnStmt * element) override;
};
