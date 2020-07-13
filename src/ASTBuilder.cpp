#include "ASTBuilder.h"
#include <vector>

using namespace antlrcpp;

using namespace AST;

ASTBuilder::ASTBuilder(TIPParser *p) : parser{p} {}

/*
 * Questions:
 *   ? when exactly do I need to use std::move
 *   ? seems like Kaleidoscope is using it too frequently
 *        (in ctor and in ctor invocatin)
 */

std::string ASTBuilder::opString(int op) {
  std::string opStr;
  switch (op) {
  case TIPParser::MUL:
    opStr = "*";
    break;
  case TIPParser::DIV:
    opStr = "/";
    break;
  case TIPParser::ADD:
    opStr = "+";
    break;
  case TIPParser::SUB:
    opStr = "-";
    break;
  case TIPParser::GT:
    opStr = ">";
    break;
  case TIPParser::EQ:
    opStr = "==";
    break;
  case TIPParser::NE:
    opStr = "!=";
    break;
  default:
    std::runtime_error(
        "unknown operator :" +
        ASTBuilder::parser->getVocabulary().getLiteralName(op));
  }
  return opStr;
}

/*
 * Globals for communicating information up from visited subtrees
 * These are overwritten by every visit call.
 * We use multiple variables here to avoid downcasting of unique smart pointers.
 */
static std::unique_ptr<Stmt> visitedStmt = nullptr;
static std::unique_ptr<DeclNode> visitedDeclNode = nullptr;
static std::unique_ptr<DeclStmt> visitedDeclStmt = nullptr;
static std::unique_ptr<Expr> visitedExpr = nullptr;
static std::unique_ptr<FieldExpr> visitedFieldExpr = nullptr;
static std::unique_ptr<Function> visitedFunction = nullptr;

static bool buildError = false;

/**********************************************************************
 * These methods override selected methods in the TIPBaseVisitor.
 *
 * For each rule name in an ANTLR4 grammar a visit method and a
 * "context" class is generated.  The visit methods are defined in
 * TIPBaseVisitor.h and the context classes in TIPParser.h
 *
 * Only the methods for grammar rules that "directly" contain
 * content that must be processed need to be overridden.  For example,
 * for strings that match "program" you need to process the list of
 * functions.
 *
 * If you are new to visitors, it is very important to understand
 * that since you are selectively overriding methods you cannot
 * rely on the call stack to return values (since the values will
 * be lost by the methods you don't override).  Instead you must create
 * your own structure that is local to the visitor to communicate between
 * the calls during the visit.  In the case of this visitor it is the
 * visitedX variables above.
 *
 * Note that the visit methods are required to return a value, but
 * we make no use of that value, so we simply return the empty string ("")
 * since we cannot return a nullptr (leads to a crash in the visitor).
 *
 * Consult context class definitions to understand the type definitions
 * of the fields that contain the program elements captured during the parse.
 * You will access these from the method overrides in your visitor.
 */

std::optional<std::unique_ptr<AST::Program>>
ASTBuilder::build(TIPParser::ProgramContext *ctx) {
  buildError = false;
  std::vector<std::unique_ptr<Function>> pFunctions;
  for (auto fn : ctx->function()) {
    visit(fn);
    pFunctions.push_back(std::move(visitedFunction));
  }
  return buildError ? std::nullopt :
         std::make_optional<std::unique_ptr<Program>>(
             std::make_unique<Program>(std::move(pFunctions)));
}

Any ASTBuilder::visitFunction(TIPParser::FunctionContext *ctx) {
  std::unique_ptr<DeclNode> fName;
  std::vector<std::unique_ptr<DeclNode>> fParams;
  std::vector<std::unique_ptr<DeclStmt>> fDecls;
  std::vector<std::unique_ptr<Stmt>> fBody;

  bool firstId = true;
  for (auto decl : ctx->nameDeclaration()) {
    visit(decl);
    if (firstId) {
      firstId = !firstId;
      fName = std::move(visitedDeclNode);
    } else {
      fParams.push_back(std::move(visitedDeclNode));
    }
  }

  for (auto decl : ctx->declaration()) {
    visit(decl);
    fDecls.push_back(std::move(visitedDeclStmt));
  }

  for (auto stmt : ctx->statement()) {
    visit(stmt);
    fBody.push_back(std::move(visitedStmt));
  }

  // return statement is always the last statement in a TIP function body
  visit(ctx->returnStmt());
  fBody.push_back(std::move(visitedStmt));

  visitedFunction = std::make_unique<Function>(
      std::move(fName), std::move(fParams), std::move(fDecls), std::move(fBody));
  return "";
}

Any ASTBuilder::visitNegNumber(TIPParser::NegNumberContext *ctx) {
  int val = std::stoi(ctx->NUMBER()->getText());
  val = -val;
  visitedExpr = std::make_unique<NumberExpr>(val);
  return "";
}

/*
 * Unfortunately, the context types for binary expressions generated
 * by ANTLR are not organized into a sub-type hierarchy.  If they were
 * we could have a single method to construct binary expression nodes,
 * but as it stands we have some repetitive code to handle the different
 * context types.
 *
 * This might be improved by restructuring the grammar, but then another
 * mechanism for handling operator precedence would be needed.
 */
Any ASTBuilder::visitAdditiveExpr(TIPParser::AdditiveExprContext *ctx) {
  std::string op = opString(ctx->op->getType());

  visit(ctx->expr(0));
  std::unique_ptr<Expr> lhs = std::move(visitedExpr);

  visit(ctx->expr(1));
  std::unique_ptr<Expr> rhs = std::move(visitedExpr);

  visitedExpr =
      std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
  return "";
}

Any ASTBuilder::visitRelationalExpr(TIPParser::RelationalExprContext *ctx) {
  std::string op = opString(ctx->op->getType());

  visit(ctx->expr(0));
  std::unique_ptr<Expr> lhs = std::move(visitedExpr);

  visit(ctx->expr(1));
  std::unique_ptr<Expr> rhs = std::move(visitedExpr);

  visitedExpr =
      std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
  return "";
}

Any ASTBuilder::visitMultiplicativeExpr(
    TIPParser::MultiplicativeExprContext *ctx) {
  std::string op = opString(ctx->op->getType());

  visit(ctx->expr(0));
  std::unique_ptr<Expr> lhs = std::move(visitedExpr);

  visit(ctx->expr(1));
  std::unique_ptr<Expr> rhs = std::move(visitedExpr);

  visitedExpr =
      std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
  return "";
}

Any ASTBuilder::visitEqualityExpr(TIPParser::EqualityExprContext *ctx) {
  std::string op = opString(ctx->op->getType());

  visit(ctx->expr(0));
  std::unique_ptr<Expr> lhs = std::move(visitedExpr);

  visit(ctx->expr(1));
  std::unique_ptr<Expr> rhs = std::move(visitedExpr);

  visitedExpr =
      std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
  return "";
}

Any ASTBuilder::visitParenExpr(TIPParser::ParenExprContext *ctx) {
  visit(ctx->expr());
  // leave visitedExpr from expr unchanged
  return "";
}

Any ASTBuilder::visitNumExpr(TIPParser::NumExprContext *ctx) {
  int val = std::stoi(ctx->NUMBER()->getText());
  visitedExpr = std::make_unique<NumberExpr>(val);
  return "";
}

Any ASTBuilder::visitVarExpr(TIPParser::VarExprContext *ctx) {
  std::string name = ctx->IDENTIFIER()->getText();
  visitedExpr = std::make_unique<VariableExpr>(name);
  return "";
}

Any ASTBuilder::visitInputExpr(TIPParser::InputExprContext *ctx) {
  visitedExpr = std::make_unique<InputExpr>();
  return "";
}

Any ASTBuilder::visitFunAppExpr(TIPParser::FunAppExprContext *ctx) {
  std::unique_ptr<Expr> fExpr = nullptr;
  std::vector<std::unique_ptr<Expr>> fArgs;

  // First expression is the function, the rest are the args
  bool first = true; 
  for (auto e : ctx->expr()) {
    visit(e);
    if (first) {
      fExpr = std::move(visitedExpr);
      first = false;
    } else {
      fArgs.push_back(std::move(visitedExpr));
    }
  }

  visitedExpr = std::make_unique<FunAppExpr>(std::move(fExpr), std::move(fArgs));
  return "";
}

Any ASTBuilder::visitAllocExpr(TIPParser::AllocExprContext *ctx) {
  visit(ctx->expr());
  visitedExpr = std::make_unique<AllocExpr>(std::move(visitedExpr));
  return "";
}

Any ASTBuilder::visitRefExpr(TIPParser::RefExprContext *ctx) {
  visit(ctx->varExpr());
  visitedExpr = std::make_unique<RefExpr>(std::move(visitedExpr));
  return "";
}

Any ASTBuilder::visitDeRefExpr(TIPParser::DeRefExprContext *ctx) {
  visit(ctx->expr());
  visitedExpr = std::make_unique<DeRefExpr>(std::move(visitedExpr));
  return "";
}

Any ASTBuilder::visitNullExpr(TIPParser::NullExprContext *ctx) {
  visitedExpr = std::make_unique<NullExpr>();
  return "";
}

Any ASTBuilder::visitRecordExpr(TIPParser::RecordExprContext *ctx) {
  std::vector<std::unique_ptr<FieldExpr>> rFields;
  for (auto fn : ctx->fieldExpr()) {
    visit(fn);
    rFields.push_back(std::move(visitedFieldExpr));
  }

  visitedExpr = std::make_unique<RecordExpr>(std::move(rFields));
  return "";
}

Any ASTBuilder::visitFieldExpr(TIPParser::FieldExprContext *ctx) {
  std::string fName = ctx->IDENTIFIER()->getText();
  visit(ctx->expr());
  visitedFieldExpr = std::make_unique<FieldExpr>(fName, std::move(visitedExpr));
  return "";
}

Any ASTBuilder::visitAccessExpr(TIPParser::AccessExprContext *ctx) {
  std::string fName = ctx->IDENTIFIER()->getText();

  visit(ctx->expr());
  std::unique_ptr<Expr> rExpr = std::move(visitedExpr);

  visitedExpr = std::make_unique<AccessExpr>(std::move(rExpr), fName);
  return "";
}

Any ASTBuilder::visitDeclaration(TIPParser::DeclarationContext *ctx) {
  std::vector<std::unique_ptr<DeclNode>> dVars;
  for (auto decl : ctx->nameDeclaration()) {
    visit(decl);
    dVars.push_back(std::move(visitedDeclNode));
  }
  visitedDeclStmt = std::make_unique<DeclStmt>(std::move(dVars));
  return "";
}

Any ASTBuilder::visitNameDeclaration(TIPParser::NameDeclarationContext *ctx) {
  std::string name = ctx->IDENTIFIER()->getText();
  visitedDeclNode = std::make_unique<DeclNode>(name);
  return "";
}

Any ASTBuilder::visitAssignmentStmt(TIPParser::AssignmentStmtContext *ctx) {
  visit(ctx->expr(0));
  std::unique_ptr<Expr> lhs = std::move(visitedExpr);
  visit(ctx->expr(1));
  std::unique_ptr<Expr> rhs = std::move(visitedExpr);
  visitedStmt = std::make_unique<AssignStmt>(std::move(lhs), std::move(rhs));
  return "";
}

Any ASTBuilder::visitBlockStmt(TIPParser::BlockStmtContext *ctx) {
  std::vector<std::unique_ptr<Stmt>> bStmts;
  for (auto s : ctx->statement()) {
    visit(s);
    bStmts.push_back(std::move(visitedStmt));
  }
  visitedStmt = std::make_unique<BlockStmt>(std::move(bStmts));
  return "";
}

Any ASTBuilder::visitWhileStmt(TIPParser::WhileStmtContext *ctx) {
  visit(ctx->expr());
  std::unique_ptr<Expr> cond = std::move(visitedExpr);

  visit(ctx->statement());
  std::unique_ptr<Stmt> body = std::move(visitedStmt);

  visitedStmt = std::make_unique<WhileStmt>(std::move(cond), std::move(body));
  return "";
}

Any ASTBuilder::visitIfStmt(TIPParser::IfStmtContext *ctx) {
  visit(ctx->expr());
  std::unique_ptr<Expr> cond = std::move(visitedExpr);

  visit(ctx->statement(0));
  std::unique_ptr<Stmt> thenBody = std::move(visitedStmt);

  // else is optional
  std::unique_ptr<Stmt> elseBody = nullptr;
  if (ctx->statement().size() == 2) {
    visit(ctx->statement(1));
    elseBody = std::move(visitedStmt);
  }

  visitedStmt = std::make_unique<IfStmt>(std::move(cond), std::move(thenBody),
                                          std::move(elseBody));
  return "";
}

Any ASTBuilder::visitOutputStmt(TIPParser::OutputStmtContext *ctx) {
  visit(ctx->expr());
  visitedStmt = std::make_unique<OutputStmt>(std::move(visitedExpr));
  return "";
}

Any ASTBuilder::visitErrorStmt(TIPParser::ErrorStmtContext *ctx) {
  visit(ctx->expr());
  visitedStmt = std::make_unique<ErrorStmt>(std::move(visitedExpr));
  return "";
}

Any ASTBuilder::visitReturnStmt(TIPParser::ReturnStmtContext *ctx) {
  visit(ctx->expr());
  visitedStmt = std::make_unique<ReturnStmt>(std::move(visitedExpr));
  return "";
}
