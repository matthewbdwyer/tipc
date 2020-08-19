#include "TipMu.h"

#include "TipInt.h"
#include "TipVar.h"
#include "catch.hpp"
#include <memory>
#include <sstream>

TEST_CASE("TipMu: test TipMus are compared by their underlying t and v" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    ASTNumberExpr n(42);
    auto var = std::make_shared<TipVar>(&n);

    TipMu mu(var, term);
    TipMu mu2(var, term);
    REQUIRE(mu == mu2);
}

TEST_CASE("TipMu: test not equals" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    ASTNumberExpr n(41);
    ASTNumberExpr n2(42);
    auto var = std::make_shared<TipVar>(&n);
    auto var2 = std::make_shared<TipVar>(&n2);

    TipMu mu(var, term);
    TipMu mu2(var2, term);
    REQUIRE(mu != mu2);
}

TEST_CASE("TipMu: test comparison with a different type" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    ASTNumberExpr n(41);
    auto var = std::make_shared<TipVar>(&n);
    TipMu mu(var, term);

    TipInt tipInt;

    REQUIRE_FALSE(mu == tipInt);
}

TEST_CASE("TipMu: test Getters", "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    ASTNumberExpr n(42);
    auto var = std::make_shared<TipVar>(&n);
    TipMu mu(var, term);
    std::stringstream stream;
    stream << mu;

    auto expected = "μ[[42@0:0]].int";
    auto actual = stream.str();

    REQUIRE(expected == actual);
}
