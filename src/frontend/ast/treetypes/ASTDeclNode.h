#pragma once

#include "ASTNode.h"

/*! \brief Class for declaring a name, e.g., function, parameter, variable
 */
class ASTDeclNode : public ASTNode {
  std::string NAME;
public:
  ASTDeclNode(std::string NAME) : NAME(NAME) {}
  std::string getName() const { return NAME; }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
