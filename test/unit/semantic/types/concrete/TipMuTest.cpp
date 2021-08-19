#include "TipMu.h"
#include "TipInt.h"
#include "TipVar.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

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

    REQUIRE(term == mu.getT());
    REQUIRE(var == mu.getV());
}

TEST_CASE("TipMu: test output stream", "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    ASTNumberExpr n(42);
    auto var = std::make_shared<TipVar>(&n);
    TipMu mu(var, term);
    std::stringstream stream;
    stream << mu;

    auto actual = stream.str();
    REQUIRE_THAT(actual, Catch::Matchers::Matches("^μ\u27E642@\\d+:\\d+\u27E7\\.int$"));
}
