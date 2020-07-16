#include "catch.hpp"
#include "TypeConstraint.h"
#include "TipFunction.h"
#include "TipInt.h"
#include <vector>

TEST_CASE("constraint are compared term-wise", "[TypeConstraint]") {
    std::vector<std::shared_ptr<TipType>> args;
    args.push_back(std::make_shared<TipInt>());
    auto function = std::make_shared<TipFunction>(args, std::make_shared<TipInt>());


    TypeConstraint constraint(function, function);
    TypeConstraint constraint2(function, function);
    REQUIRE(constraint == constraint2);
    REQUIRE_FALSE(constraint != constraint2);
}

