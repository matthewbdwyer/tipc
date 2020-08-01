#include "catch.hpp"
#include "TipAlpha.h"
#include <sstream>

TEST_CASE("TipAlpha: Test TipAlphas are compared by their underlying objects" "[TipAlpha]") {
    TipAlpha tipAlphaA;
    TipAlpha tipAlphaB;
    REQUIRE(tipAlphaA == tipAlphaB);
}

TEST_CASE("TipAlpha: Test TipAlphas are compared by their underlying objects 2" "[TipAlpha]") {
    TipAlpha tipAlphaA("foo");
    TipAlpha tipAlphaB("bar");
    REQUIRE_FALSE(tipAlphaA == tipAlphaB);
}

TEST_CASE("TipAlpha: Test getter" "[TipAlpha]") {
    TipAlpha tipAlphaA("foo");
    TipAlpha tipAlphaB("bar");

    std::string expectedValueA("foo");
    std::string expectedValueB("bar");

    REQUIRE(expectedValueA == tipAlphaA.getAlphaValue());
    REQUIRE(expectedValueB == tipAlphaB.getAlphaValue());
}

TEST_CASE("TipAlpha: Test output stream" "[TipAlpha]") {
    TipAlpha tipAlphaA("foo");

    std::string expectedValueA("\u03B1<foo>");

    std::stringstream stream;
    stream << tipAlphaA;
    std::string actualValueA = stream.str();

    REQUIRE(expectedValueA == actualValueA);
}
