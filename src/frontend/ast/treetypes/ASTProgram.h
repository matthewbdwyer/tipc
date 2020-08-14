#pragma once

#include "ASTFunction.h"

class SemanticAnalysis;

/*! \brief Class for a program which is a name and a list of functions.
 *
 * This class is not part of the ASTNode hierarchy though it does share
 * some of the function signatures so that it can participate in visitors.
 * \sa ASTNode
 */
class ASTProgram {
  std::string name;
  std::vector<std::unique_ptr<ASTFunction>> FUNCTIONS;
public:
  ASTProgram(std::vector<std::unique_ptr<ASTFunction>> FUNCTIONS)
      : FUNCTIONS(std::move(FUNCTIONS)) {}
  void setName(std::string n) { name = n; }
  std::string getName() const { return name; }
  std::vector<ASTFunction*> getFunctions() const;
  ASTFunction * findFunctionByName(std::string);
  void accept(ASTVisitor * visitor);
  std::unique_ptr<llvm::Module> codegen(SemanticAnalysis* st, std::string name);
};
