#include "catch.hpp"
#include "TipCons.h"
#include "TipVar.h"

class MockCons1: public TipCons {
public:
    MockCons1() : TipCons(std::vector<std::shared_ptr<TipType>> {nullptr}) {};
    bool operator==(const TipType &other) const override { return true; }
    bool operator!=(const TipType &other) const override { return false; }
    std::ostream &print(std::ostream &out) const override { return out; }
};

class MockCons2: public TipCons {
public:
    MockCons2() : TipCons(std::vector<std::shared_ptr<TipType>> {nullptr, nullptr}) {};
    bool operator==(const TipType &other) const override { return true; }
    bool operator!=(const TipType &other) const override { return false; }
    std::ostream &print(std::ostream &out) const override { return out; }
};

TEST_CASE("TipCons: Test doMatch considers arity" "[TipCons]") {
    MockCons1 mockCons1a;
    MockCons1 mockCons1b;

    MockCons2 mockCons2a;
    MockCons2 mockCons2b;

    REQUIRE(mockCons1a.doMatch(&mockCons1b));
    REQUIRE(mockCons2a.doMatch(&mockCons2b));
    REQUIRE_FALSE(mockCons1a.doMatch(&mockCons2a));
}

TEST_CASE("TipCons: Test doMatch only works on TipCons" "[TipCons]") {
    MockCons1 mockCons1;
    TipVar tipVar;

    REQUIRE_FALSE(mockCons1.doMatch(&tipVar));
}

