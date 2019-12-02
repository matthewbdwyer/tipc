#include "TiptreeDepthFirstVisitor.h"

void TiptreeDepthFirstVisitor::visitChildren(Node *node) {
  if (auto *expr = dynamic_cast<Expr*>(node)) {
    visitChildren(expr);
  } else if (auto *stmt = dynamic_cast<Stmt*>(node)) {
    visitChildren(stmt);
  }
}

void TiptreeDepthFirstVisitor::visitChildren(Expr *expr) {
  if (auto *be = dynamic_cast<BinaryExpr*>(expr)) {
    visitChildren(be);
  } else if (auto *fe = dynamic_cast<FunAppExpr*>(expr)) {
    visitChildren(fe);
  } else if (auto *ie = dynamic_cast<InputExpr*>(expr)) {
    visitChildren(ie);
  } else if (auto *de = dynamic_cast<DeRefExpr*>(expr)) {
    visitChildren(de);
  }
}

void TiptreeDepthFirstVisitor::visitChildren(Stmt *stmt) {
  if (auto *bs = dynamic_cast<BlockStmt*>(stmt)) {
    visitChildren(bs);
  } else if (auto *as = dynamic_cast<AssignStmt*>(stmt)) {
    visitChildren(as);
  } else if (auto *ws = dynamic_cast<WhileStmt*>(stmt)) {
    visitChildren(ws);
  } else if (auto *is = dynamic_cast<IfStmt*>(stmt)) {
    visitChildren(is);
  } else if (auto *os = dynamic_cast<OutputStmt*>(stmt)) {
    visitChildren(os);
  } else if (auto *rs = dynamic_cast<ReturnStmt*>(stmt)) {
    visitChildren(rs);
  }
}

void TiptreeDepthFirstVisitor::visitChildren(BinaryExpr *expr) {
  visit(expr->getLhs().get());
  visit(expr->getRhs().get());
}

void TiptreeDepthFirstVisitor::visitChildren(FunAppExpr *expr) {
  visit(expr->getFun().get());
  for (auto &actual : expr->getActuals()) {
    visit(actual.get());
  }
}

void TiptreeDepthFirstVisitor::visitChildren(DeRefExpr *expr) {
  visit(expr->getArg().get());
}

void TiptreeDepthFirstVisitor::visitChildren(BlockStmt *bs) {
  for (auto &stmt : bs->getStmts()) {
    visit(stmt.get());
  }
}

void TiptreeDepthFirstVisitor::visitChildren(AssignStmt *as) {
  visit(as->getLhs().get());
  visit(as->getRhs().get());
}

void TiptreeDepthFirstVisitor::visitChildren(WhileStmt *ws) {
  visit(ws->getCond().get());
  visit(ws->getBody().get());
}

void TiptreeDepthFirstVisitor::visitChildren(IfStmt *is) {
  visit(is->getCond().get());
  visit(is->getThen().get());
  visit(is->getElse().get());
}

void TiptreeDepthFirstVisitor::visitChildren(OutputStmt *os) {
  visit(os->getArg().get());
}

void TiptreeDepthFirstVisitor::visitChildren(ReturnStmt *rs) {
  visit(rs->getArg().get());
}


void TiptreeDepthFirstVisitor::visitChildren(Program *pm) {
  for (auto &fn : pm->getFunctions()) {
    visit(fn.get());
  }
}

void TiptreeDepthFirstVisitor::visitChildren(Function *fn) {
  for (auto &decl : fn->getDecls()) {
    visit(decl.get());
  }

  for (auto &stmt : fn->getBody()) {
    visit(stmt.get());
  }
}
