#include "TipAlpha.h"
#include "ASTNumberExpr.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE("TipAlpha: Test TipAlphas are compared by their underlying objects"
          "[TipAlpha]") {
  ASTNumberExpr num(13);
  TipAlpha tipAlphaA(&num);
  TipAlpha tipAlphaB(&num);
  REQUIRE(tipAlphaA == tipAlphaB);
}

TEST_CASE("TipAlpha: TipAlpha equality with context and names"
          "[TipAlpha]") {
  ASTNumberExpr num1(13);
  ASTNumberExpr num2(42);
  ASTNumberExpr num3(7);

  TipAlpha tipAlpha1(&num1);
  TipAlpha tipAlpha1foo(&num1, "foo");
  TipAlpha tipAlpha1bar(&num1, "bar");
  TipAlpha tipAlpha21foo(&num2, &num1, "foo");
  TipAlpha tipAlpha12bar(&num1, &num2, "bar");
  TipAlpha tipAlpha21foo2(&num2, &num1, "foo");
  TipAlpha tipAlpha13bar(&num1, &num3, "bar");
  TipAlpha tipAlpha21bar(&num2, &num1, "bar");

  REQUIRE_FALSE(tipAlpha1 == tipAlpha1bar);      // implicit name mismatch
  REQUIRE_FALSE(tipAlpha1bar == tipAlpha1foo);   // explicit name mismatch
  REQUIRE_FALSE(tipAlpha1bar == tipAlpha12bar);  // context mismatch
  REQUIRE_FALSE(tipAlpha21foo == tipAlpha21bar); // context match, name mismatch
  REQUIRE_FALSE(tipAlpha12bar == tipAlpha13bar); // context mismatch, name match
  REQUIRE(tipAlpha21foo == tipAlpha21foo2);
}

TEST_CASE("TipAlpha: Test getter"
          "[TipAlpha]") {
  ASTNumberExpr num1(13);
  ASTNumberExpr num2(42);
  TipAlpha tipAlphaA(&num1);
  TipAlpha tipAlphaB(&num2);

  auto node1 = dynamic_cast<ASTNumberExpr *>(tipAlphaA.getNode());
  auto node2 = dynamic_cast<ASTNumberExpr *>(tipAlphaB.getNode());
  REQUIRE(node1->getValue() == 13);
  REQUIRE(node2->getValue() == 42);
}

TEST_CASE("TipAlpha: Test output stream"
          "[TipAlpha]") {
  ASTNumberExpr num(13);
  TipAlpha tipAlphaA(&num);

  std::string expectedValueA("\u03B1<13@0:0>");

  std::stringstream stream;
  stream << tipAlphaA;
  std::string actualValueA = stream.str();

  REQUIRE(expectedValueA == actualValueA);
}
