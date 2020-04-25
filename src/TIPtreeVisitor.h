#ifndef TIPC_TIPTREEVISITOR_H
#define TIPC_TIPTREEVISITOR_H
#include "TIPtree.h"

class TIPtreeVisitor {
public:
  virtual void visit(TIPtree::Program * element)  = 0;
  virtual void visit(TIPtree::Function * element) = 0;
  virtual void visit(TIPtree::NumberExpr * element) = 0;
  virtual void visit(TIPtree::VariableExpr * element) = 0;
  virtual void visit(TIPtree::BinaryExpr * element) = 0;
  virtual void visit(TIPtree::InputExpr * element) = 0;
  virtual void visit(TIPtree::FunAppExpr * element) = 0;
  virtual void visit(TIPtree::AllocExpr * element) = 0;
  virtual void visit(TIPtree::RefExpr * element) = 0;
  virtual void visit(TIPtree::DeRefExpr * element) = 0;
  virtual void visit(TIPtree::NullExpr * element) = 0;
  virtual void visit(TIPtree::FieldExpr * element) = 0;
  virtual void visit(TIPtree::RecordExpr * element) = 0;
  virtual void visit(TIPtree::AccessExpr * element) = 0;
  virtual void visit(TIPtree::DeclStmt * element) = 0;
  virtual void visit(TIPtree::AssignStmt * element) = 0;
  virtual void visit(TIPtree::WhileStmt * element) = 0;
  virtual void visit(TIPtree::IfStmt * element) = 0;
  virtual void visit(TIPtree::OutputStmt * element) = 0;
  virtual void visit(TIPtree::ReturnStmt * element) = 0;
  virtual void visit(TIPtree::ErrorStmt * element) = 0;
  virtual void visit(TIPtree::BlockStmt * element) = 0;
};


#endif //TIPC_TIPTREEVISITOR_H
