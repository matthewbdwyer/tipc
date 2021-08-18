#include "ASTHelper.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("ASTProgramTest: ASTProgram output is the hash of the source.", "[ASTProgram]") {
  std::stringstream stream;
  stream << R"(
      foo(x) {
         return x + 1;
      }

      main() {
        var z;
        z = foo(42);
        return z;
      }
    )";

  auto ast = ASTHelper::build_ast(stream);

  std::string expectedOutput = "29e272ab0b8752287594fb0014953a9cf7879e3687f4d481ced352a835b46359";
  std::stringstream actualOutput;
  actualOutput << *ast;
  REQUIRE(expectedOutput == actualOutput.str());
}

TEST_CASE("ASTProgramTest: ASTProgram returns nullptr when getting nonexistent function", "[ASTProgram]") {
  std::stringstream stream;
  stream << R"(
      foo(x) { return x; }
      bar(x) { return x; }
      baz(x) { return x; }
    )";

  auto ast = ASTHelper::build_ast(stream);
  ASTFunction * expectedFunction = nullptr;
  ASTFunction * actualFunction = ast->findFunctionByName("fred");
  REQUIRE(expectedFunction == actualFunction);
}
