#pragma once

#include "ASTNode.h"

//! \brief Base class for all expression subtypes
class ASTExpr : public ASTNode {
public:
  ~ASTExpr() = default;
  // delegating the obligation to override accept and codegen functions
};

