#include "catch.hpp"
#include "TipAlpha.h"

TEST_CASE("TipAlpha: test TipAlphas are compared by their underlying objects" "[TipAlpha]") {
    TipAlpha tipAlpha;
    TipAlpha tipAlpha2;
    REQUIRE(tipAlpha == tipAlpha2);
}

TEST_CASE("TipAlpha: test TipAlphas are compared by their underlying objects 2" "[TipAlpha]") {
    TipAlpha tipAlpha("foo");
    TipAlpha tipAlpha2("bar");
    REQUIRE_FALSE(tipAlpha == tipAlpha2);
}

