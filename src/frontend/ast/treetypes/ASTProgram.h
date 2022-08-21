#pragma once

#include "ASTFunction.h"
#include <ostream>

class SemanticAnalysis;

/*! \brief Class for a program which is a name and a list of functions.
 *
 */
class ASTProgram: public ASTNode {
  std::string name;
  std::vector<std::shared_ptr<ASTFunction>> FUNCTIONS;
public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTProgram(std::vector<std::unique_ptr<ASTFunction>> FUNCTIONS);
  void setName(std::string n) { name = n; }
  std::string getName() const { return name; }
  std::vector<ASTFunction*> getFunctions() const;
  ASTFunction * findFunctionByName(std::string);
  void accept(ASTVisitor * visitor) override;
  std::unique_ptr<llvm::Module> codegen(SemanticAnalysis* st, std::string name);

private:
  llvm::Value *codegen() override;

public:

  friend std::ostream& operator<<(std::ostream& os, const ASTProgram& obj) {
    return obj.print(os);
  }

protected:
  std::ostream& print(std::ostream &out) const override;
};
