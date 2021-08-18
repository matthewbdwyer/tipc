#include "TipInt.h"
#include "TipRef.h"
#include "TipFunction.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE("TipRef: Test TipRefs are compared by their underlying term" "[TipRef]") {
    auto term = std::make_shared<TipInt>();
    TipRef tipRef(term);

    SECTION("Equal when terms are the same") {
        auto term = std::make_shared<TipInt>();
        TipRef tipRef2(term);
        REQUIRE(tipRef == tipRef2);
    }

    SECTION("Not equal when terms are different") {
        std::vector<std::shared_ptr<TipType>> params {
                std::make_shared<TipInt>(),
                std::make_shared<TipInt>()
        };
        auto ret = std::make_shared<TipRef>(std::make_shared<TipInt>());
        auto tipFunction = std::make_shared<TipFunction>(params, ret);
        TipRef tipRef2(tipFunction);

        REQUIRE_FALSE(tipRef == tipRef2);
    }

}

TEST_CASE("TipRef: Test arity is one" "[TipRef]") {
    auto term = std::make_shared<TipInt>();
    TipRef tipRef(term);
    REQUIRE(1 == tipRef.arity());
}

TEST_CASE("TipRef: Test getter" "[TipRef]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipRef>(std::make_shared<TipInt>());
    auto tipFunction = std::make_shared<TipFunction>(params, ret);
    TipRef tipRef(tipFunction);

    REQUIRE(*tipFunction == *tipRef.getAddressOfField());
}

TEST_CASE("TipRef: Test output stream" "[TipRef]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipInt>();
    auto tipFunction = std::make_shared<TipFunction>(params, ret);
    TipRef tipRef(tipFunction);

    auto expectedValue = "&(int,int) -> int";
    std::stringstream stream;
    stream << tipRef;
    std::string actualValue = stream.str();

    REQUIRE(expectedValue == actualValue);
}
