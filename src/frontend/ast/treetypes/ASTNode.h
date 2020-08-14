#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <string>
#include <vector>
#include <ostream>

// Forward declare the visitor to resolve circular dependency
class ASTVisitor;

//! \brief Base class for all AST nodes.
class ASTNode {
  int line;
  int column;
public:
  virtual ~ASTNode() = default;
  virtual void accept(ASTVisitor * visitor) = 0;
  virtual llvm::Value* codegen() = 0;

  void setLocation(int l, int c) { line = l; column = c; }
  int getLine() { return line; }
  int getColumn() { return column; }

  friend std::ostream& operator<<(std::ostream& os, const ASTNode& obj) {
    return obj.print(os);
  }

protected:
  virtual std::ostream& print(std::ostream &out) const = 0;
};
