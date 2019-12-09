#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "TIPtreeTypes.h"

/******************************************************************
 * Abstract Syntax Tree for TIP
 *****************************************************************/

namespace TIPtree {

// Node - this is a base class for all tree nodes
class Node {
public:
  virtual ~Node() = default;
  virtual llvm::Value *codegen() = 0;
  virtual std::string print() = 0;
};

/******************* Expression AST Nodes *********************/

// Expr - Base class for all expression nodes.
class Expr : public Node {
public:
  ~Expr() = default;
  // delegating the obligation to override the functions
};

// NumberExpr - Expression class for numeric literals
class NumberExpr : public Expr {
  int VAL;

public:
  NumberExpr(int VAL) : VAL(VAL) {}
  llvm::Value *codegen() override;
  std::string print() override;
};

/// VariableExpr - class for referencing a variable
class VariableExpr : public Expr {
  std::string NAME;

public:
  VariableExpr(const std::string &NAME) : NAME(NAME) {}
  llvm::Value *codegen() override;
  std::string print() override;
  // Getter to distinguish LHS of assigment for codegen
  std::string getName() { return NAME; };
};

/// BinaryExpr - class for a binary operator.
class BinaryExpr : public Expr {
  std::string OP;
  std::unique_ptr<Expr> LHS, RHS;

public:
  BinaryExpr(const std::string &OP, std::unique_ptr<Expr> LHS,
             std::unique_ptr<Expr> RHS)
      : OP(OP), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::string getOp() { return OP; };
  std::unique_ptr<Expr> &getLhs() { return LHS; };
  std::unique_ptr<Expr> &getRhs() { return RHS; };
};

/// FunAppExpr - class for function calls.
class FunAppExpr : public Expr {
  std::unique_ptr<Expr> FUN;
  std::vector<std::unique_ptr<Expr>> ACTUALS;

public:
  FunAppExpr(std::unique_ptr<Expr> FUN,
             std::vector<std::unique_ptr<Expr>> ACTUALS)
      : FUN(std::move(FUN)), ACTUALS(std::move(ACTUALS)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getFun() { return FUN; };
  std::vector<std::unique_ptr<Expr>> &getActuals() { return ACTUALS; };
};

/// InputExpr - class for input expression
class InputExpr : public Expr {

public:
  InputExpr() {}
  llvm::Value *codegen() override;
  std::string print() override;
};

// AllocExpr - class for alloc expression
class AllocExpr : public Expr {
  std::unique_ptr<Expr> ARG;

public:
  AllocExpr(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getArg() { return ARG; };
};

// RefExpr - class for referencing the address of a variable
class RefExpr : public Expr {
  std::string NAME;

public:
  RefExpr(const std::string &NAME) : NAME(NAME) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::string getName() { return NAME; };
};

// DeRefExpr - class for dereferencing a pointer expression
class DeRefExpr : public Expr {
  std::unique_ptr<Expr> ARG;

public:
  DeRefExpr(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getArg() { return ARG; };
};

/// NullExpr - class for a null expression
class NullExpr : public Expr {

public:
  NullExpr() {}
  llvm::Value *codegen() override;
  std::string print() override;
};

// FieldExpr - class for the field of a structure
class FieldExpr : public Expr {
  std::string FIELD;
  std::unique_ptr<Expr> INIT;

public:
  FieldExpr(const std::string &FIELD, std::unique_ptr<Expr> INIT)
      : FIELD(FIELD), INIT(std::move(INIT)) {}
  llvm::Value *codegen() override;
  std::string print() override;
};

// RecordExpr - class for defining a record
class RecordExpr : public Expr {
  std::vector<std::unique_ptr<FieldExpr>> FIELDS;

public:
  RecordExpr(std::vector<std::unique_ptr<FieldExpr>> FIELDS)
      : FIELDS(std::move(FIELDS)) {}
  llvm::Value *codegen() override;
  std::string print() override;
};

// AccessExpr - class for a record field access
class AccessExpr : public Expr {
  std::unique_ptr<Expr> RECORD;
  std::string FIELD;

public:
  AccessExpr(std::unique_ptr<Expr> RECORD, const std::string &FIELD)
      : RECORD(std::move(RECORD)), FIELD(FIELD) {}
  llvm::Value *codegen() override;
  std::string print() override;
};

/******************* Statement AST Nodes *********************/

// Stmt - Base class for all statement nodes.
class Stmt : public Node {
public:
  ~Stmt() = default;
  // delegating the obligation to override the functions
};

// DeclStmt - class for declaration
class DeclStmt : public Stmt {
  std::vector<std::string> VARS;
  std::vector<std::shared_ptr<TIPtreeTypes::Type>> types;
  int LINE; // line on which decl statement occurs

public:
  DeclStmt(std::vector<std::string> VARS, int LINE)
      : VARS(std::move(VARS)), LINE(LINE) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::vector<std::string> getVars() { return VARS; };
  void setTypes(std::vector<std::shared_ptr<TIPtreeTypes::Type>> ts) { types = ts; };
};

// BlockStmt - class for block of statements
class BlockStmt : public Stmt {
  std::vector<std::unique_ptr<Stmt>> STMTS;

public:
  BlockStmt(std::vector<std::unique_ptr<Stmt>> STMTS)
      : STMTS(std::move(STMTS)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::vector<std::unique_ptr<Stmt>> &getStmts() { return STMTS; };
};

// AssignStmt - class for assignment
class AssignStmt : public Stmt {
  std::unique_ptr<Expr> LHS, RHS;

public:
  AssignStmt(std::unique_ptr<Expr> LHS, std::unique_ptr<Expr> RHS)
      : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getLhs() { return LHS; };
  std::unique_ptr<Expr> &getRhs() { return RHS; };
};

// WhileStmt - class for a while loop
class WhileStmt : public Stmt {
  std::unique_ptr<Expr> COND;
  std::unique_ptr<Stmt> BODY;

public:
  WhileStmt(std::unique_ptr<Expr> COND, std::unique_ptr<Stmt> BODY)
      : COND(std::move(COND)), BODY(std::move(BODY)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getCond() { return COND; };
  std::unique_ptr<Stmt> &getBody() { return BODY; };
};

/// IfStmt - class for if-then-else
class IfStmt : public Stmt {
  std::unique_ptr<Expr> COND;
  std::unique_ptr<Stmt> THEN, ELSE;

public:
  IfStmt(std::unique_ptr<Expr> COND, std::unique_ptr<Stmt> THEN,
         std::unique_ptr<Stmt> ELSE)
      : COND(std::move(COND)), THEN(std::move(THEN)), ELSE(std::move(ELSE)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getCond() { return COND; };
  std::unique_ptr<Stmt> &getThen() { return THEN; };
  std::unique_ptr<Stmt> &getElse() { return ELSE; };
};

/// OutputStmt - class for a output statement
class OutputStmt : public Stmt {
  std::unique_ptr<Expr> ARG;

public:
  OutputStmt(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getArg() { return ARG; };
};

/// ErrorStmt - class for a error statement
class ErrorStmt : public Stmt {
  std::unique_ptr<Expr> ARG;

public:
  ErrorStmt(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  llvm::Value *codegen() override;
  std::string print() override;
};

/// ReturnStmt - class for a return statement
class ReturnStmt : public Stmt {
  std::unique_ptr<Expr> ARG;

public:
  ReturnStmt(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  llvm::Value *codegen() override;
  std::string print() override;

  std::unique_ptr<Expr> &getArg() { return ARG; };
};

/******************* Program and Function Nodes *********************/

// Function - signature, local declarations, and a body
class Function {
  std::string NAME;
  std::vector<std::string> FORMALS;
  std::vector<std::unique_ptr<DeclStmt>> DECLS;
  std::vector<std::unique_ptr<Stmt>> BODY;
  std::shared_ptr<TIPtreeTypes::Type> type;
  int LINE; // line on which function definition occurs

public:
  Function(const std::string &NAME, std::vector<std::string> FORMALS,
           std::vector<std::unique_ptr<DeclStmt>> DECLS,
           std::vector<std::unique_ptr<Stmt>> BODY, int LINE)
      : NAME(NAME), FORMALS(std::move(FORMALS)), DECLS(std::move(DECLS)),
        BODY(std::move(BODY)), LINE(LINE) {}
  llvm::Function *codegen();
  std::string print();

  /*
   * These getters are needed because we perform two passes over
   * functions during code generation:
   *   1) a shallow pass that declares all function signatures
   *   2) a deep pass that generates function implementations
   * The getters support first pass.
   */
  std::string getName() { return NAME; };
  std::vector<std::string> getFormals() { return FORMALS; };
  std::vector<std::unique_ptr<DeclStmt>> &getDecls() { return DECLS; };
  std::vector<std::unique_ptr<Stmt>> &getBody() { return BODY; };
  // return statement is always the last statement in a TIP function body
  ReturnStmt* getReturn() { return dynamic_cast<ReturnStmt*>(BODY.back().get()); };
  void setType(std::shared_ptr<TIPtreeTypes::Type> t) { type = t; };
};

// Program - just a list of functions
class Program {
  std::vector<std::unique_ptr<Function>> FUNCTIONS;

public:
  Program(std::vector<std::unique_ptr<Function>> FUNCTIONS)
      : FUNCTIONS(std::move(FUNCTIONS)) {}
  std::unique_ptr<llvm::Module> codegen(std::string programName);
  std::string print(std::string i, bool pl, bool pt);

  std::vector<std::unique_ptr<Function>> &getFunctions() { return FUNCTIONS; };
};

} // namespace TIPtree
