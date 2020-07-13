#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <string>
#include <vector>

// Forward declare the visitor to resolve circular dependency
class ASTVisitor;

/******************************************************************
 * Abstract Syntax Tree for TIP
 *****************************************************************/
namespace AST {

// Node - this is a base class for all tree nodes
class Node {
public:
  virtual ~Node() = default;
  virtual void accept(ASTVisitor * visitor) = 0;
  virtual llvm::Value* codegen() = 0;
};

/******************* Declaration AST Nodes *********************/

/// DeclNode - class for declaring a name, e.g., function, parameter, variable
class DeclNode : public Node {
  std::string NAME;
public:
  DeclNode(std::string NAME) : NAME(NAME) {}
  std::string getName() { return NAME; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
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
  int getValue() { return VAL; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// VariableExpr - class for referencing a variable
class VariableExpr : public Expr {
  std::string NAME;
public:
  VariableExpr(std::string NAME) : NAME(NAME) {}
  std::string getName() { return NAME; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// BinaryExpr - class for a binary operator.
class BinaryExpr : public Expr {
  std::string OP;
  std::unique_ptr<Expr> LEFT, RIGHT;
public:
  BinaryExpr(const std::string &OP, std::unique_ptr<Expr> LEFT,
             std::unique_ptr<Expr> RIGHT)
      : OP(OP), LEFT(std::move(LEFT)), RIGHT(std::move(RIGHT)) {}
  std::string getOp() { return OP; }
  Expr* getLeft() { return LEFT.get(); }
  Expr* getRight() { return RIGHT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// FunAppExpr - class for function calls.
class FunAppExpr : public Expr {
  std::unique_ptr<Expr> FUN;
  std::vector<std::unique_ptr<Expr>> ACTUALS;
public:
  FunAppExpr(std::unique_ptr<Expr> FUN,
             std::vector<std::unique_ptr<Expr>> ACTUALS)
      : FUN(std::move(FUN)), ACTUALS(std::move(ACTUALS)) {}
  Expr* getFunction() { return FUN.get(); }
  std::vector<Expr*> getActuals();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// InputExpr - class for input expression
class InputExpr : public Expr {
public:
  InputExpr() {}
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// AllocExpr - class for alloc expression
class AllocExpr : public Expr {
  std::unique_ptr<Expr> INIT;
public:
  AllocExpr(std::unique_ptr<Expr> INIT) : INIT(std::move(INIT)) {}
  Expr* getInitializer() { return INIT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// RefExpr - class for referencing the address of a variable
class RefExpr : public Expr {
  std::unique_ptr<Expr> VAR;
public:
  RefExpr(std::unique_ptr<Expr> VAR) : VAR(std::move(VAR)) {}
  Expr* getVar() { return VAR.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// DeRefExpr - class for dereferencing a pointer expression
class DeRefExpr : public Expr {
  std::unique_ptr<Expr> PTR;
public:
  DeRefExpr(std::unique_ptr<Expr> PTR) : PTR(std::move(PTR)) {}
  Expr* getPtr() { return PTR.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// NullExpr - class for a null expression
class NullExpr : public Expr {
public:
  NullExpr() {}
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// FieldExpr - class for the field of a structure
class FieldExpr : public Expr {
  std::string FIELD;
  std::unique_ptr<Expr> INIT;
public:
  FieldExpr(const std::string &FIELD, std::unique_ptr<Expr> INIT)
      : FIELD(FIELD), INIT(std::move(INIT)) {}
  std::string getField() { return FIELD; }
  Expr* getInitializer() { return INIT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// RecordExpr - class for defining a record
class RecordExpr : public Expr {
  std::vector<std::unique_ptr<FieldExpr>> FIELDS;
public:
  RecordExpr(std::vector<std::unique_ptr<FieldExpr>> FIELDS)
      : FIELDS(std::move(FIELDS)) {}
  std::vector<FieldExpr*> getFields();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// AccessExpr - class for a record field access
class AccessExpr : public Expr {
  std::unique_ptr<Expr> RECORD;
  std::string FIELD;
public:
  AccessExpr(std::unique_ptr<Expr> RECORD, const std::string &FIELD)
      : RECORD(std::move(RECORD)), FIELD(FIELD) {}
  std::string getField() { return FIELD; }
  Expr* getRecord() { return RECORD.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
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
  std::vector<std::unique_ptr<DeclNode>> VARS;
public:
  DeclStmt(std::vector<std::unique_ptr<DeclNode>> VARS) : VARS(std::move(VARS)){}
  std::vector<DeclNode*> getVars();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// BlockStmt - class for block of statements
class BlockStmt : public Stmt {
  std::vector<std::unique_ptr<Stmt>> STMTS;
public:
  BlockStmt(std::vector<std::unique_ptr<Stmt>> STMTS)
      : STMTS(std::move(STMTS)) {}
  std::vector<Stmt*> getStmts();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// AssignStmt - class for assignment
class AssignStmt : public Stmt {
  std::unique_ptr<Expr> LHS, RHS;
public:
  AssignStmt(std::unique_ptr<Expr> LHS, std::unique_ptr<Expr> RHS)
      : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  Expr* getLHS() { return LHS.get(); }
  Expr* getRHS() { return RHS.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// WhileStmt - class for a while loop
class WhileStmt : public Stmt {
  std::unique_ptr<Expr> COND;
  std::unique_ptr<Stmt> BODY;
public:
  WhileStmt(std::unique_ptr<Expr> COND, std::unique_ptr<Stmt> BODY)
      : COND(std::move(COND)), BODY(std::move(BODY)) {}
  Expr* getCondition() { return COND.get(); }
  Stmt* getBody() { return BODY.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// IfStmt - class for if-then-else
class IfStmt : public Stmt {
  std::unique_ptr<Expr> COND;
  std::unique_ptr<Stmt> THEN, ELSE;
public:
  IfStmt(std::unique_ptr<Expr> COND, std::unique_ptr<Stmt> THEN,
         std::unique_ptr<Stmt> ELSE)
      : COND(std::move(COND)), THEN(std::move(THEN)), ELSE(std::move(ELSE)) {}
  Expr* getCondition() { return COND.get(); }
  Stmt* getThen() { return THEN.get(); }
  Stmt* getElse() { return ELSE.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// OutputStmt - class for a output statement
class OutputStmt : public Stmt {
  std::unique_ptr<Expr> ARG;
public:
  OutputStmt(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  Expr* getArg() { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// ErrorStmt - class for a error statement
class ErrorStmt : public Stmt {
  std::unique_ptr<Expr> ARG;
public:
  ErrorStmt(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  Expr* getArg() { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/// ReturnStmt - class for a return statement
class ReturnStmt : public Stmt {
  std::unique_ptr<Expr> ARG;
public:
  ReturnStmt(std::unique_ptr<Expr> ARG) : ARG(std::move(ARG)) {}
  Expr* getArg() { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

/******************* Program and Function Nodes *********************/

// Function - signature, local declarations, and a body
class Function : public Node {
  std::unique_ptr<DeclNode> DECL;
  std::vector<std::unique_ptr<DeclNode>> FORMALS;
  std::vector<std::unique_ptr<DeclStmt>> DECLS;
  std::vector<std::unique_ptr<Stmt>> BODY;
public:
  Function(std::unique_ptr<DeclNode> DECL, 
           std::vector<std::unique_ptr<DeclNode>> FORMALS,
           std::vector<std::unique_ptr<DeclStmt>> DECLS,
           std::vector<std::unique_ptr<Stmt>> BODY)
      : DECL(std::move(DECL)), FORMALS(std::move(FORMALS)), 
        DECLS(std::move(DECLS)), BODY(std::move(BODY)) {}
  DeclNode* getDecl() { return DECL.get(); };
  std::string getName() { return DECL->getName(); };
  std::vector<DeclNode*> getFormals();
  std::vector<DeclStmt*> getDeclarations();
  std::vector<Stmt*> getStmts();
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};

// Program - a name and a list of functions
class Program {
  std::string name;
  std::vector<std::unique_ptr<Function>> FUNCTIONS;
public:
  Program(std::vector<std::unique_ptr<Function>> FUNCTIONS)
      : FUNCTIONS(std::move(FUNCTIONS)) {}
  void setName(std::string n) { name = n; }
  std::string getName() { return name; }
  std::vector<Function*> getFunctions();
  void accept(ASTVisitor * visitor);
  std::unique_ptr<llvm::Module> codegen(std::string name);
};

} // namespace AST

