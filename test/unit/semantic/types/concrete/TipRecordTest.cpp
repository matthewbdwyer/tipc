#include "TipInt.h"
#include "TipRef.h"
#include "TipRecord.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <iostream>

TEST_CASE("TipRecord: Test getters" "[TipRecord]") {
    std::vector<std::shared_ptr<TipType>> inits {
            std::make_shared<TipInt>(),
            std::make_shared<TipRef>(std::make_shared<TipInt>())
    };
    std::vector<std::string> names {"foo", "bar"};
    TipRecord tipRecord(inits, names);

    REQUIRE(names.size() == tipRecord.getNames().size());
    REQUIRE(names.front() == tipRecord.getNames().front());
    REQUIRE(names.back() == tipRecord.getNames().back());
    REQUIRE(inits.size() == tipRecord.getInits().size());
    REQUIRE(dynamic_cast<const TipInt *>(tipRecord.getInits().front().get()));
    REQUIRE(dynamic_cast<const TipRef *>(tipRecord.getInits().back().get()));
}

TEST_CASE("TipRecord: Test arity" "[TipRecord]") {
    std::vector<std::shared_ptr<TipType>> inits {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>(),
    };
    std::vector<std::string> names {"foo", "bar", "baz", "freddie", "fannie"};
    TipRecord tipRecord(inits, names);

    REQUIRE(5 == tipRecord.arity());
}

TEST_CASE("TipRecord: Test equality" "[TipRecord]") {
    std::vector<std::shared_ptr<TipType>> initsA {
            std::make_shared<TipInt>(),
            std::make_shared<TipRef>(std::make_shared<TipInt>())
    };
    std::vector<std::string> namesA {"foo", "bar"};
    TipRecord tipRecordA(initsA, namesA);

    SECTION("Equal when fields are of same type and length") {
        std::vector<std::shared_ptr<TipType>> initsB {
                std::make_shared<TipInt>(),
                std::make_shared<TipRef>(std::make_shared<TipInt>())
        };
        std::vector<std::string> namesB {"foo", "bar"};
        TipRecord tipRecordB(initsB, namesB);

        REQUIRE(tipRecordA == tipRecordB);
    }

    SECTION("Not equal when arguments differ by length") {
        std::vector<std::shared_ptr<TipType>> initsB {
                std::make_shared<TipInt>(),
                std::make_shared<TipRef>(std::make_shared<TipInt>()),
                std::make_shared<TipRef>(std::make_shared<TipInt>())
        };
        std::vector<std::string> namesB {"foo", "bar"};
        TipRecord tipRecordB(initsB, namesB);

        REQUIRE(tipRecordA != tipRecordB);
    }

    SECTION("Not equal when arguments differ by type") {
        std::vector<std::shared_ptr<TipType>> initsB {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>(),
        };
        std::vector<std::string> namesB {"foo", "bar"};
        TipRecord tipRecordB(initsB, namesB);

        REQUIRE(tipRecordA != tipRecordB);
    }

    SECTION("Not equal when argument types differ") {
        TipInt tipInt;

        REQUIRE(tipRecordA != tipInt);
    }
}

TEST_CASE("TipRecord: Test output stream" "[TipRecord]") {
    std::vector<std::shared_ptr<TipType>> inits {
            std::make_shared<TipInt>(),
            std::make_shared<TipRef>(std::make_shared<TipInt>())
    };
    std::vector<std::string> names {"foo", "bar"};
    TipRecord tipRecord(inits, names);

    auto expectedValue = "{foo:int,bar:\u2B61int}";
    std::stringstream stream;
    stream << tipRecord;
    std::string actualValue = stream.str();

    REQUIRE(expectedValue == actualValue);
}
