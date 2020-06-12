#pragma once

#include "ASTVisitor.h"
#include <ostream>
#include <iostream>
#include <string>

class PrettyPrinter: public ASTVisitor {
public:
  PrettyPrinter() : os(std::cout), indentChar(' '), indentSize(2), printLines(true) {}
  PrettyPrinter(std::ostream &os, char indentChar, int indentSize, bool printLines) :
          os(os), indentChar(indentChar), indentSize(indentSize), printLines(printLines) {}

  static void print(std::unique_ptr<AST::Program> p, 
                    std::ostream &s, 
                    char c, int n, bool wlines);

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

private:
  std::string indent() const;
  void safePrintLineNumber(int lineNumber) const;

  int indentLevel = 0;

  std::ostream &os;
  char indentChar;
  int indentSize;
  bool printLines;
};

