#include "TipInt.h"
#include "TipRef.h"
#include "TipFunction.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <iostream>

TEST_CASE("TipFunction: Test getters" "[TipFunction]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipRef>(std::make_shared<TipInt>());
    TipFunction tipFunction(params, ret);

    REQUIRE(2 == tipFunction.getParams().size());
    REQUIRE(*ret == *tipFunction.getReturnValue());
}

TEST_CASE("TipFunction: Test equality" "[TipFunction]") {
    std::vector<std::shared_ptr<TipType>> paramsA {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto retA = std::make_shared<TipInt>();
    TipFunction tipFunctionA(paramsA, retA);

    SECTION("Equal when arguments and return value are of same type and length") {
        std::vector<std::shared_ptr<TipType>> paramsB {
                std::make_shared<TipInt>(),
                std::make_shared<TipInt>()
        };
        auto retB = std::make_shared<TipInt>();
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA == tipFunctionB);
    }

    SECTION("Not equal when arguments differ by length") {
        std::vector<std::shared_ptr<TipType>> paramsB {std::make_shared<TipInt>()};
        auto retB = std::make_shared<TipInt>();
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA != tipFunctionB);
    }

    SECTION("Not equal when arguments differ by type") {
        std::vector<std::shared_ptr<TipType>> paramsB {
            std::make_shared<TipInt>(),
            std::make_shared<TipRef>(std::make_shared<TipInt>())
        };
        auto retB = std::make_shared<TipInt>();
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA != tipFunctionB);
    }

    SECTION("Not equal when return values differ by type") {
        std::vector<std::shared_ptr<TipType>> paramsB {
                std::make_shared<TipInt>(),
                std::make_shared<TipInt>()
        };
        auto retB = std::make_shared<TipRef>(std::make_shared<TipInt>());
        TipFunction tipFunctionB(paramsB, retB);
        REQUIRE(tipFunctionA != tipFunctionB);
    }
}

TEST_CASE("TipFunction: Test output stream" "[TipFunction]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipRef>(std::make_shared<TipInt>());
    TipFunction tipFunction(params, ret);

    auto expectedValue = "(int,int) -> &int";
    std::stringstream stream;
    stream << tipFunction;
    std::string actualValue = stream.str();

    REQUIRE(expectedValue == actualValue);
}
