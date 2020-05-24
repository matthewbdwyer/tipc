#pragma once

#include "TIPtreeVisitor.h"

/**
 * Sample Visitor to illustrate traversing the AST.
 */
class SampleVisitor: public TIPtreeVisitor {
public:
  void visit(TIPtree::Program * element) override;
  void visit(TIPtree::Function * element) override;
  void visit(TIPtree::NumberExpr * element) override;
  void visit(TIPtree::VariableExpr * element) override;
  void visit(TIPtree::BinaryExpr * element) override;
  void visit(TIPtree::InputExpr * element) override;
  void visit(TIPtree::FunAppExpr * element) override;
  void visit(TIPtree::AllocExpr * element) override;
  void visit(TIPtree::RefExpr * element) override;
  void visit(TIPtree::DeRefExpr * element) override;
  void visit(TIPtree::NullExpr * element) override;
  void visit(TIPtree::FieldExpr * element) override;
  void visit(TIPtree::RecordExpr * element) override;
  void visit(TIPtree::AccessExpr * element) override;
  void visit(TIPtree::DeclStmt * element) override;
  void visit(TIPtree::AssignStmt * element) override;
  void visit(TIPtree::BlockStmt * element) override;
  void visit(TIPtree::WhileStmt * element) override;
  void visit(TIPtree::IfStmt * element) override;
  void visit(TIPtree::OutputStmt * element) override;
  void visit(TIPtree::ErrorStmt * element) override;
  void visit(TIPtree::ReturnStmt * element) override;
};
