#include "TipVar.h"
#include "TipInt.h"

#include <catch2/catch_test_macros.hpp>

#include <vector>

TEST_CASE("TipVar: check equality with the same underlying node are equal"
          "[TipVar]") {
  ASTNumberExpr n(42);
  TipVar var(&n);
  TipVar var2(&n);
  REQUIRE(var == var2);
}

TEST_CASE("TipVar: check equality with different underlying nodes"
          "[TipVar]") {
  ASTNumberExpr n(99);
  ASTNumberExpr n1(99);
  TipVar var(&n);
  TipVar var2(&n1);
  REQUIRE_FALSE(var == var2);
}

TEST_CASE("TipVar: test TipVar is a TipType"
          "[TipVar]") {
  ASTNumberExpr n(42);
  TipVar var(&n);
  REQUIRE_FALSE(nullptr == dynamic_cast<TipType *>(&var));
}
