
// Generated from TIP.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "TIPVisitor.h"


/**
 * This class provides an empty implementation of TIPVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  TIPBaseVisitor : public TIPVisitor {
public:

  virtual antlrcpp::Any visitProgram(TIPParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunction(TIPParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNegNumber(TIPParser::NegNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtomExpr(TIPParser::AtomExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditiveExpr(TIPParser::AdditiveExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelationalExpr(TIPParser::RelationalExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMultiplicativeExpr(TIPParser::MultiplicativeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEqualityExpr(TIPParser::EqualityExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParenAtom(TIPParser::ParenAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunAppAtom(TIPParser::FunAppAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumExpr(TIPParser::NumExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdExpr(TIPParser::IdExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInputExpr(TIPParser::InputExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAllocExpr(TIPParser::AllocExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRefExpr(TIPParser::RefExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeRefAtom(TIPParser::DeRefAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNullExpr(TIPParser::NullExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRecordAtom(TIPParser::RecordAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParenExpr(TIPParser::ParenExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeRefExpr(TIPParser::DeRefExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunAppExpr(TIPParser::FunAppExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRecordExpr(TIPParser::RecordExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFieldExpr(TIPParser::FieldExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAccessExpr(TIPParser::AccessExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignableExpr(TIPParser::AssignableExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclaration(TIPParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatement(TIPParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignmentStmt(TIPParser::AssignmentStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlockStmt(TIPParser::BlockStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileStmt(TIPParser::WhileStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfStmt(TIPParser::IfStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOutputStmt(TIPParser::OutputStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitErrorStmt(TIPParser::ErrorStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnStmt(TIPParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }


};

