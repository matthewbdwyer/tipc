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
    std::vector<std::unique_ptr<ASTDeclNode>> formals;
    std::vector<std::unique_ptr<ASTDeclStmt>> decls;
    std::vector<std::unique_ptr<ASTStmt>> body;
    ASTFunction mockFunction(
        std::make_unique<ASTDeclNode>(fname),
        std::move(formals),
        std::move(decls),
        std::move(body)
    );
    return std::move(mockFunction);
  }

}