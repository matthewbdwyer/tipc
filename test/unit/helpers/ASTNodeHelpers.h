#pragma once
#include "AST.h"

namespace nullcodegen {
  class MockASTExpr: public ASTExpr {
  public:
    llvm::Value *codegen() override {
      return nullptr;
    }

    void accept(ASTVisitor *visitor) override {

    }

    std::ostream &print(std::ostream &out) const override {
      return out;
    }
  };

  class MockASTStmt: public ASTStmt {
  public:
    llvm::Value *codegen() override {
      return nullptr;
    }

    void accept(ASTVisitor *visitor) override {

    }

    std::ostream &print(std::ostream &out) const override {
      return out;
    }
  };

}

namespace simplenodes {

  ASTFunction mockFunction(std::string fname) {
    std::vector<std::shared_ptr<ASTDeclNode>> formals;
    std::vector<std::shared_ptr<ASTDeclStmt>> decls;
    std::vector<std::shared_ptr<ASTStmt>> body;
    std::vector<std::shared_ptr<ASTExpr>> rets;
    ASTFunction mockFunction(
        std::make_shared<ASTDeclNode>(fname),
        formals,
        decls,
        body,
        rets
    );
    return mockFunction;
  }

}
