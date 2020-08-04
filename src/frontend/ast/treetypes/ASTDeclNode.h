#pragma once

#include "ASTNode.h"

// DeclNode - class for declaring a name, e.g., function, parameter, variable
class ASTDeclNode : public ASTNode {
  std::string NAME;
public:
  ASTDeclNode(std::string NAME) : NAME(NAME) {}
  std::string getName() { return NAME; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
