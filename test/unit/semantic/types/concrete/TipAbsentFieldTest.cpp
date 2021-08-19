#include "TipAbsentField.h"
#include "TipVar.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>

TEST_CASE("TipAbsentField: test TipAbsentField is a TipCons" "[tip_absentfield]") {
    TipAbsentField t;
    REQUIRE_FALSE(nullptr == dynamic_cast<TipCons *>(&t));
}

TEST_CASE("TipAbsentField: test TipAbsentField is a TipType" "[tip_absentfield]") {
    TipAbsentField t;
    REQUIRE_FALSE(nullptr == dynamic_cast<TipType *>(&t));
}

TEST_CASE("TipAbsentField: test args is empty", "[tip_absentfield]") {
    TipAbsentField t;
    REQUIRE(t.getArguments().empty());
}

TEST_CASE("TipAbsentField: test toString returns int", "[tip_absentfield]") {
    TipAbsentField t;
    std::stringstream stream;
    stream << t;
    REQUIRE("\u25C7" == stream.str());
}

TEST_CASE("TipAbsentField: test all TipAbsentFields are equal", "[tip_absentfield]") {
    TipAbsentField t1;
    TipAbsentField t2;
    REQUIRE(t1 == t2);
}
