#include "TIPtree.h"

using namespace std;
using namespace TIPtree;


class TIPtreeDepthFirstVisitor {
public:
  virtual void visit(Program *pm) = 0;
  virtual void visit(Function *fn) = 0;

  virtual void visit(Node *node) = 0;
  virtual void visit(Expr *expr) = 0;
  virtual void visit(Stmt *stmt) = 0;

protected:
  void visitChildren(Program *pm);
  void visitChildren(Function *fn);

  void visitChildren(Node *node);
  void visitChildren(Expr *expr);
  void visitChildren(Stmt *stmt);

  void visitChildren(BinaryExpr *expr);
  void visitChildren(FunAppExpr *expr);
  void visitChildren(DeRefExpr *expr);
  void visitChildren(AllocExpr *expr);
  void visitChildren(BlockStmt *bs);
  void visitChildren(AssignStmt *as);
  void visitChildren(WhileStmt *ws);
  void visitChildren(IfStmt *is);
  void visitChildren(OutputStmt *os);
  void visitChildren(ReturnStmt *stmt);
};
