#include "ASTHelper.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("ASTAccessExprTest: Test methods of AST subtype.",
          "[ASTAccessExpr]") {
  std::stringstream stream;
  stream << R"(
      foo() {
         return {f : 0}.f;
      }
    )";

  auto ast = ASTHelper::build_ast(stream);
  auto accexpr = ASTHelper::find_node<ASTAccessExpr>(ast);

  std::stringstream o1;
  o1 << accexpr->getField();
  REQUIRE(o1.str() == "f");

  std::stringstream o2;
  o2 << accexpr->getRecord();
  REQUIRE(o2.str() == "{f : 0}");
}
