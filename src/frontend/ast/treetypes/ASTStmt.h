#pragma once

#include "ASTNode.h"

/*! \brief Base class for all statement nodes.
 */
class ASTStmt : public ASTNode {
public:
  ~ASTStmt() = default;
  // delegating the obligation to override the accept, codegen and print 
};
