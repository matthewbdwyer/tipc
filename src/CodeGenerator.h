#pragma once

#include "ASTVisitor.h"
#include <ostream>
#include <iostream>
#include <string>

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

class CodeGenerator: public ASTVisitor {
public:
  CodeGenerator() {} 

  // Main entry point for code generation
  static std::unique_ptr<llvm::Module> codegen(
             std::unique_ptr<AST::Program> p, std::string pname);

  // TBD Can visit methods be private?
  void visit(AST::Program * element) override;
  void visit(AST::Function * element) override;
  void visit(AST::NumberExpr * element) override;
  void visit(AST::VariableExpr * element) override;
  void visit(AST::BinaryExpr * element) override;
  void visit(AST::InputExpr * element) override;
  void visit(AST::FunAppExpr * element) override;
  void visit(AST::AllocExpr * element) override;
  void visit(AST::RefExpr * element) override;
  void visit(AST::DeRefExpr * element) override;
  void visit(AST::NullExpr * element) override;
  void visit(AST::FieldExpr * element) override;
  void visit(AST::RecordExpr * element) override;
  void visit(AST::AccessExpr * element) override;
  void visit(AST::DeclStmt * element) override;
  void visit(AST::AssignStmt * element) override;
  void visit(AST::BlockStmt * element) override;
  void visit(AST::WhileStmt * element) override;
  void visit(AST::IfStmt * element) override;
  void visit(AST::OutputStmt * element) override;
  void visit(AST::ErrorStmt * element) override;
  void visit(AST::ReturnStmt * element) override;

private:
/*
 * Create LLVM Function in Module associated with current program.
 * This function declares the function, but it does not generate code.
 * This is a key element of the shallow pass that builds the function
 * dispatch table.
 */
llvm::Function *getFunction(std::string Name);

/*
 * Create an alloca instruction in the entry block of the function.
 * This is used for mutable variables, including arguments to functions.
 */
llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction,
                                         const std::string &VarName);

llvm::Value *LogError(std::string s);

};

