#include "ASTHelper.h"
#include "SyntaxTree.h"
#include "Iterator.h"
#include "PrettyPrinter.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>


class MockVisitor: public ASTVisitor {};

TEST_CASE("SyntaxTree: Test assignment updates root on different AST", "[SyntaxTree]") {
  std::stringstream streamX;
  streamX << R"(
      progx() {
        var x;
        x = 1;
        return z;
      }
    )";
  std::shared_ptr<ASTProgram> astX = std::move(ASTHelper::build_ast(streamX));
  SyntaxTree syntaxTreeX(astX);

  std::stringstream streamY;
  streamY << R"(
      progy() {
        var y;
        y = 1;
        return z;
      }
    )";
  std::shared_ptr<ASTProgram> astY = std::move(ASTHelper::build_ast(streamY));
  SyntaxTree syntaxTreeY(astY);

  syntaxTreeX = syntaxTreeY;
  REQUIRE(syntaxTreeX.getRoot() == syntaxTreeY.getRoot());
}

TEST_CASE("SyntaxTree: Test assignment checks root", "[SyntaxTree]") {
  std::stringstream stream;
  stream << R"(
      progx() {
        var x;
        x = 1;
        return z;
      }
    )";
  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTreeX(ast);
  SyntaxTree syntaxTreeY(ast);

  REQUIRE_FALSE(&syntaxTreeX == &syntaxTreeY);

  // If the underlying root is the same do not do anything.
  syntaxTreeX = syntaxTreeY;
  REQUIRE_FALSE(&syntaxTreeX == &syntaxTreeY);
}

TEST_CASE("SyntaxTree: Test traversing a tree with a visitor", "[SyntaxTree]") {
  std::stringstream stream;
  stream << R"(
      progx() {
        var x;
        x = 1;
        return z;
      }
    )";
  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTreeX(ast);

  // Assert visiting logic is independent of iterating.
  MockVisitor mockVisitor;
  for(auto iter = syntaxTreeX.begin(""); iter != syntaxTreeX.end(""); ++iter) {
    iter->accept(&mockVisitor);
  }
  REQUIRE(true);
}