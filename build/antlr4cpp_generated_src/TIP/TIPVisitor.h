
// Generated from TIP.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "TIPParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by TIPParser.
 */
class  TIPVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by TIPParser.
   */
    virtual antlrcpp::Any visitProgram(TIPParser::ProgramContext *context) = 0;

    virtual antlrcpp::Any visitFunction(TIPParser::FunctionContext *context) = 0;

    virtual antlrcpp::Any visitNegNumber(TIPParser::NegNumberContext *context) = 0;

    virtual antlrcpp::Any visitAtomExpr(TIPParser::AtomExprContext *context) = 0;

    virtual antlrcpp::Any visitAdditiveExpr(TIPParser::AdditiveExprContext *context) = 0;

    virtual antlrcpp::Any visitRelationalExpr(TIPParser::RelationalExprContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicativeExpr(TIPParser::MultiplicativeExprContext *context) = 0;

    virtual antlrcpp::Any visitEqualityExpr(TIPParser::EqualityExprContext *context) = 0;

    virtual antlrcpp::Any visitParenAtom(TIPParser::ParenAtomContext *context) = 0;

    virtual antlrcpp::Any visitFunAppAtom(TIPParser::FunAppAtomContext *context) = 0;

    virtual antlrcpp::Any visitNumExpr(TIPParser::NumExprContext *context) = 0;

    virtual antlrcpp::Any visitIdExpr(TIPParser::IdExprContext *context) = 0;

    virtual antlrcpp::Any visitInputExpr(TIPParser::InputExprContext *context) = 0;

    virtual antlrcpp::Any visitAllocExpr(TIPParser::AllocExprContext *context) = 0;

    virtual antlrcpp::Any visitRefExpr(TIPParser::RefExprContext *context) = 0;

    virtual antlrcpp::Any visitDeRefAtom(TIPParser::DeRefAtomContext *context) = 0;

    virtual antlrcpp::Any visitNullExpr(TIPParser::NullExprContext *context) = 0;

    virtual antlrcpp::Any visitRecordAtom(TIPParser::RecordAtomContext *context) = 0;

    virtual antlrcpp::Any visitParenExpr(TIPParser::ParenExprContext *context) = 0;

    virtual antlrcpp::Any visitDeRefExpr(TIPParser::DeRefExprContext *context) = 0;

    virtual antlrcpp::Any visitFunAppExpr(TIPParser::FunAppExprContext *context) = 0;

    virtual antlrcpp::Any visitRecordExpr(TIPParser::RecordExprContext *context) = 0;

    virtual antlrcpp::Any visitFieldExpr(TIPParser::FieldExprContext *context) = 0;

    virtual antlrcpp::Any visitAccessExpr(TIPParser::AccessExprContext *context) = 0;

    virtual antlrcpp::Any visitAssignableExpr(TIPParser::AssignableExprContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(TIPParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitStatement(TIPParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitAssignmentStmt(TIPParser::AssignmentStmtContext *context) = 0;

    virtual antlrcpp::Any visitBlockStmt(TIPParser::BlockStmtContext *context) = 0;

    virtual antlrcpp::Any visitWhileStmt(TIPParser::WhileStmtContext *context) = 0;

    virtual antlrcpp::Any visitIfStmt(TIPParser::IfStmtContext *context) = 0;

    virtual antlrcpp::Any visitOutputStmt(TIPParser::OutputStmtContext *context) = 0;

    virtual antlrcpp::Any visitErrorStmt(TIPParser::ErrorStmtContext *context) = 0;

    virtual antlrcpp::Any visitReturnStmt(TIPParser::ReturnStmtContext *context) = 0;


};

