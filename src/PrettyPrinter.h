#pragma once

#include "ASTVisitor.h"
#include <ostream>
#include <iostream>
#include <string>

/**
 * GNU Style PrettyPrinter.
 */
class PrettyPrinter: public ASTVisitor {
public:
  PrettyPrinter() : os(std::cout), indentChar(' '), indentSize(2) {}
  PrettyPrinter(std::ostream &os, char indentChar, int indentSize) :
          os(os), indentChar(indentChar), indentSize(indentSize) {}

  static void print(AST::Program* p, std::ostream &s, char c, int n);
  void print2(AST::Node *p, std::ostream &s, char c, int n);
  virtual void endVisitNode(AST::Node * element);

  virtual void endVisit(AST::Program * element) override;
  virtual bool visit(AST::Function * element) override;
  virtual void endVisit(AST::Function * element) override;
  virtual void endVisit(AST::NumberExpr * element) override;
  virtual void endVisit(AST::VariableExpr * element) override;
  virtual void endVisit(AST::BinaryExpr * element) override;
  virtual void endVisit(AST::InputExpr * element) override;
  virtual void endVisit(AST::FunAppExpr * element) override;
  virtual void endVisit(AST::AllocExpr * element) override;
  virtual void endVisit(AST::RefExpr * element) override;
  virtual void endVisit(AST::DeRefExpr * element) override;
  virtual void endVisit(AST::NullExpr * element) override;
  virtual void endVisit(AST::FieldExpr * element) override;
  virtual void endVisit(AST::RecordExpr * element) override;
  virtual void endVisit(AST::AccessExpr * element) override;
  virtual void endVisit(AST::DeclNode * element) override;
  virtual void endVisit(AST::DeclStmt * element) override;
  virtual void endVisit(AST::AssignStmt * element) override;
  virtual bool visit(AST::BlockStmt * element) override;
  virtual void endVisit(AST::BlockStmt * element) override;
  virtual bool visit(AST::WhileStmt * element) override;
  virtual void endVisit(AST::WhileStmt * element) override;
  virtual bool visit(AST::IfStmt * element) override;
  virtual void endVisit(AST::IfStmt * element) override;
  virtual void endVisit(AST::OutputStmt * element) override;
  virtual void endVisit(AST::ReturnStmt * element) override;
  virtual void endVisit(AST::ErrorStmt * element) override;

private:
  std::string indent() const;
  int indentLevel = 0;
  char indentChar;
  int indentSize;
  std::ostream &os;

  /* 
   * LIFO record of strings produced by visiting sub-trees
   */
  std::vector<std::string> visitResults;
};

