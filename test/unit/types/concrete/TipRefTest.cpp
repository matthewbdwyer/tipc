#include "catch.hpp"
#include "TipInt.h"
#include "TipRef.h"

TEST_CASE("TipRef: test TipRefs are compared by their underlying term" "[TipRef]") {
    auto term = std::make_shared<TipInt>();
    TipRef ref(term);
    TipRef ref2(term);

    REQUIRE(ref == ref2);
}

