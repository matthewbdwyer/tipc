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

  static void print(ASTProgram* p, std::ostream &s, char c, int n);
  void print2(ASTNode *p, std::ostream &s, char c, int n);
  virtual void endVisitNode(ASTNode * element);

  virtual void endVisit(ASTProgram * element) override;
  virtual bool visit(ASTFunction * element) override;
  virtual void endVisit(ASTFunction * element) override;
  virtual void endVisit(ASTNumberExpr * element) override;
  virtual void endVisit(ASTVariableExpr * element) override;
  virtual void endVisit(ASTBinaryExpr * element) override;
  virtual void endVisit(ASTInputExpr * element) override;
  virtual void endVisit(ASTFunAppExpr * element) override;
  virtual void endVisit(ASTAllocExpr * element) override;
  virtual void endVisit(ASTRefExpr * element) override;
  virtual void endVisit(ASTDeRefExpr * element) override;
  virtual void endVisit(ASTNullExpr * element) override;
  virtual void endVisit(ASTFieldExpr * element) override;
  virtual void endVisit(ASTRecordExpr * element) override;
  virtual void endVisit(ASTAccessExpr * element) override;
  virtual void endVisit(ASTDeclNode * element) override;
  virtual void endVisit(ASTDeclStmt * element) override;
  virtual void endVisit(ASTAssignStmt * element) override;
  virtual bool visit(ASTBlockStmt * element) override;
  virtual void endVisit(ASTBlockStmt * element) override;
  virtual bool visit(ASTWhileStmt * element) override;
  virtual void endVisit(ASTWhileStmt * element) override;
  virtual bool visit(ASTIfStmt * element) override;
  virtual void endVisit(ASTIfStmt * element) override;
  virtual void endVisit(ASTOutputStmt * element) override;
  virtual void endVisit(ASTReturnStmt * element) override;
  virtual void endVisit(ASTErrorStmt * element) override;

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

