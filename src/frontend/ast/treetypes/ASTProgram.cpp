#include "ASTProgram.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTProgram::ASTProgram(std::vector<std::unique_ptr<ASTFunction>> FUNCTIONS) {
  for(auto &func : FUNCTIONS) {
    std::shared_ptr<ASTFunction> f = std::move(func);
    this->FUNCTIONS.push_back(f);
  }
}

std::vector<ASTFunction*> ASTProgram::getFunctions() const {
  return rawRefs(FUNCTIONS);
}

ASTFunction * ASTProgram::findFunctionByName(std::string name) {
    for(auto fn : getFunctions()) {
        if(fn->getName() == name) {
            return fn;
        }
    }
    return nullptr;
}

void ASTProgram::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto f : getFunctions()) {
      f->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream& ASTProgram::print(std::ostream &out) const {
  out << getName();
  return out;
}  // LCOV_EXCL_LINE

std::vector<std::shared_ptr<ASTNode>> ASTProgram::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  for(auto &function : FUNCTIONS) {
    children.push_back(function);
  }
  return children;
}

/* This function is never called because a custom code generation
 * routine, which accepts additional arguments, is defined for programs.
 */
llvm::Value *ASTProgram::codegen() { // LCOV_EXCL_LINE
  assert(0); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE
