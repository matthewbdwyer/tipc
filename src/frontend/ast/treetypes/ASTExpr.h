#pragma once

#include "ASTNode.h"

// Base class for all expression subtypes
class ASTExpr : public ASTNode {
public:
  ~ASTExpr() = default;
  // delegating the obligation to override accept and codegen functions
};

