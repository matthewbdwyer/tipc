#include "catch.hpp"
#include "TipInt.h"
#include "TipMu.h"
#include "TipVar.h"
#include <memory>

TEST_CASE("TipMu: test TipMus are compared by their underlying t and v" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    AST::NumberExpr n(42);
    auto var = std::make_shared<TipVar>(&n);

    TipMu mu(var, term);
    TipMu mu2(var, term);
    REQUIRE(mu == mu2);
}

TEST_CASE("TipMu: test not equals" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    AST::NumberExpr n(42);
    AST::NumberExpr n2(42);
    auto var = std::make_shared<TipVar>(&n);
    auto var2 = std::make_shared<TipVar>(&n2);

    TipMu mu(var, term);
    TipMu mu2(var2, term);
    REQUIRE(mu != mu2);
}

TEST_CASE("TipMu: test TipMu is a Mu" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    AST::NumberExpr n(42);
    auto var = std::make_shared<TipVar>(&n);

    TipMu mu(var, term);
    REQUIRE_FALSE(nullptr == dynamic_cast<Mu *>(&mu));
}

TEST_CASE("TipMu: test TipMu is a TipType" "[TipMu]") {
    auto term = std::make_shared<TipInt>();
    AST::NumberExpr n(42);
    auto var = std::make_shared<TipVar>(&n);

    TipMu mu(var, term);
    REQUIRE_FALSE(nullptr == dynamic_cast<TipType *>(&mu));
}

// TODO
TEST_CASE("TipMu: test this is returned when substitute equals member v", "[.][TipMu]") {
    //auto term = std::make_shared<TipInt>();
    //AST::NumberExpr n(42);
    //auto var = std::make_shared<TipVar>(&n);

    //TipMu mu(var, term);

    //TipInt t;

    //Term * expected = dynamic_cast<Term *>(&mu);
    //Term * actual = mu.substitute(&var, &t);
    //REQUIRE(actual == expected);
    //REQUIRE_FALSE(nullptr == dynamic_cast<Mu *>(actual));
}
