#pragma once

#include "ASTVisitor.h"
#include <ostream>
#include <iostream>
#include <string>

/*! \brief GNU Style PrettyPrinter.
 *
 * Visitor to produce a textual representation of a program represented as an AST.
 * The visitor is parameterized by the whitespace character and the preferred number
 * of those characters to use for formatting the program.  The output is written to
 * a provided std::ostream.
 *
 * There are two primary entry points to this class.  The print method prints entire
 * programs and the print2 method prints any sub-tree of the program.
 *
 * This visitor uses primarily a post-order traversal of the AST, by overriding
 * endVisit methods, and therefore uses a LIFO protocol for storing and accessing
 * values computed during the traversal, i.e., in visitResults.
 */
class PrettyPrinter: public ASTVisitor {
public:
  PrettyPrinter() : os(std::cout), indentChar(' '), indentSize(2) {}
  PrettyPrinter(std::ostream &os, char indentChar, int indentSize) :
          os(os), indentChar(indentChar), indentSize(indentSize) {}

  static void print(ASTProgram* p, std::ostream &os, char c, int n);

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

