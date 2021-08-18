#include "TipCons.h"
#include "TipRecord.h"
#include "TipRef.h"
#include "TipInt.h"
#include "TipVar.h"
#include "TipTypeVisitor.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("TipCons: Test doMatch considers arity" "[TipCons]") {
    auto tipInt = std::make_shared<TipInt>();

    std::vector<std::shared_ptr<TipType>> params0 { };
    std::vector<std::shared_ptr<TipType>> params1 { tipInt };

    // Function with no params
    auto tipFunction1 = std::make_shared<TipFunction>(params0, tipInt);

    // Function with one param
    auto tipFunction2 = std::make_shared<TipFunction>(params1, tipInt);

    REQUIRE_FALSE(tipFunction1->doMatch(tipFunction2.get()));
    REQUIRE_FALSE(tipFunction2->doMatch(tipFunction1.get()));
}

TEST_CASE("TipCons: Test doMatch considers constructor type" "[TipCons]") {
    std::vector<std::shared_ptr<TipType>> params { };

    auto tipInt = std::make_shared<TipInt>();

    // Ref is cons with 1 argument
    auto tipRef = std::make_shared<TipRef>(tipInt);

    // Function with no params is cons with one argument
    auto tipFunction = std::make_shared<TipFunction>(params, tipInt);

    std::vector<std::shared_ptr<TipType>> inits { tipInt };
    std::vector<std::string> names {"foo"};

    // Record with one field is cons with one argument
    auto tipRecord = std::make_shared<TipRecord>(inits, names);

    REQUIRE_FALSE(tipRef->doMatch(tipFunction.get()));
    REQUIRE_FALSE(tipRef->doMatch(tipRecord.get()));
    REQUIRE_FALSE(tipRecord->doMatch(tipRef.get()));
    REQUIRE_FALSE(tipRecord->doMatch(tipFunction.get()));
    REQUIRE_FALSE(tipFunction->doMatch(tipRef.get()));
    REQUIRE_FALSE(tipFunction->doMatch(tipRecord.get()));
}

TEST_CASE("TipCons: Test doMatch only works on TipCons" "[TipCons]") {
    auto tipInt = std::make_shared<TipInt>();
    auto tipRef = std::make_shared<TipRef>(tipInt);

    REQUIRE_FALSE(tipRef->doMatch(tipInt.get()));
}

