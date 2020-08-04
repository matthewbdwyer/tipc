#pragma once

#include "ASTNode.h"

// ASTStmt - Base class for all statement nodes.
class ASTStmt : public ASTNode {
public:
  ~ASTStmt() = default;
  // delegating the obligation to override the accept and codegen functions
};
