#include "TipInt.h"
#include "TipVar.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>

TEST_CASE("TipInt: test TipInt is a TipCons" "[tip_int]") {
    TipInt t;
    REQUIRE_FALSE(nullptr == dynamic_cast<TipCons *>(&t));
}

TEST_CASE("TipInt: test TipInt is a TipType" "[tip_int]") {
    TipInt t;
    REQUIRE_FALSE(nullptr == dynamic_cast<TipType *>(&t));
}

TEST_CASE("TipInt: test args is empty", "[tip_int]") {
    TipInt t;
    REQUIRE(t.getArguments().empty());
}

TEST_CASE("TipInt: test toString returns int", "[tip_int]") {
    TipInt t;
    std::stringstream stream;
    stream << t;
    REQUIRE("int" == stream.str());
}

TEST_CASE("TipInt: test all TipInts are equal", "[tip_int]") {
    TipInt t1;
    TipInt t2;
    REQUIRE(t1 == t2);
}
