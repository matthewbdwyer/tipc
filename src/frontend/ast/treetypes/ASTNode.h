#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <string>
#include <vector>

// Forward declare the visitor to resolve circular dependency
class ASTVisitor;

//! \brief Base class for all AST nodes.
class ASTNode {
public:
  virtual ~ASTNode() = default;
  virtual void accept(ASTVisitor * visitor) = 0;
  virtual llvm::Value* codegen() = 0;
};
