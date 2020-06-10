#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ASTHelper.h"
#include "GeneralHelper.h"
#include "PrettyPrinter.h"

#include <iostream>

TEST_CASE("PrettyPrinter: Test default constructor", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(
      short() {
        var x, y, z;
        x = input;
        y = alloc x;
        *y = x;
        z = *y;
        return z;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter prettyPrinter;
    ast->accept(&prettyPrinter);
    REQUIRE(true);
}

TEST_CASE("PrettyPrinter: Test indentation", "[PrettyPrinter]") {
  std::stringstream stream;
  stream << R"(
      foo() {
        var x;
        if(1) {
          x = 5;
        }
        return 5;
      }
    )";

  auto ast = ASTHelper::build_ast(stream);

  std::stringstream outputStream;
  PrettyPrinter prettyPrinter(outputStream, '#', 2, true);
  ast->accept(&prettyPrinter);
  std::vector<std::string> tokens = GeneralHelper::tokenize(outputStream.str(), '\n');

  auto token0 = tokens.at(0);
  auto token1 = tokens.at(1);
  auto token2 = tokens.at(2);
  auto token3 = tokens.at(3);
  auto token4 = tokens.at(4);
  auto token5 = tokens.at(5);
  auto token6 = tokens.at(6);
  auto token7 = tokens.at(7);
  auto token8 = tokens.at(8);
  auto token9 = tokens.at(9);

  REQUIRE(0 == std::count(token0.begin(), token0.end(), '#'));
  REQUIRE(2 == std::count(token1.begin(), token1.end(), '#'));
  REQUIRE(4 == std::count(token2.begin(), token2.end(), '#'));
  REQUIRE(4 == std::count(token3.begin(), token3.end(), '#'));
  REQUIRE(6 == std::count(token4.begin(), token4.end(), '#'));
  REQUIRE(8 == std::count(token5.begin(), token5.end(), '#'));
  REQUIRE(6 == std::count(token6.begin(), token6.end(), '#'));
  REQUIRE(4 == std::count(token7.begin(), token7.end(), '#'));
  REQUIRE(2 == std::count(token8.begin(), token8.end(), '#'));
  REQUIRE(0 == std::count(token9.begin(), token9.end(), '#'));
}
