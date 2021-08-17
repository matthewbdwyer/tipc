#include "TipAlpha.h"
#include "ASTNumberExpr.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE("TipAlpha: Test TipAlphas are compared by their underlying objects" "[TipAlpha]") {
    ASTNumberExpr num(13);
    TipAlpha tipAlphaA(&num);
    TipAlpha tipAlphaB(&num);
    REQUIRE(tipAlphaA == tipAlphaB);
}

TEST_CASE("TipAlpha: Test getter" "[TipAlpha]") {
    ASTNumberExpr num1(13);
    ASTNumberExpr num2(42);
    TipAlpha tipAlphaA(&num1);
    TipAlpha tipAlphaB(&num2);

    auto node1 = dynamic_cast<ASTNumberExpr*>(tipAlphaA.getNode());
    auto node2 = dynamic_cast<ASTNumberExpr*>(tipAlphaB.getNode());
    REQUIRE(node1->getValue() == 13);
    REQUIRE(node2->getValue() == 42);
}

TEST_CASE("TipAlpha: Test output stream" "[TipAlpha]") {
    ASTNumberExpr num(13);
    TipAlpha tipAlphaA(&num);

    std::string expectedValueA("\u03B1<13>");

    std::stringstream stream;
    stream << tipAlphaA;
    std::string actualValueA = stream.str();

    REQUIRE(expectedValueA == actualValueA);
}
