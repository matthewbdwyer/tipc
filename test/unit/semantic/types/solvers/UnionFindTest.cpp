#include "ASTNumberExpr.h"
#include "TipVar.h"
#include "UnionFind.h"

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <utility>
#include <vector>

static std::vector<std::shared_ptr<TipType>> intsToTipVars(std::vector<int> &values) {
    std::vector<std::shared_ptr<TipType>> pointers;
    for(auto &value : values) {
        ASTNumberExpr * n = new ASTNumberExpr(value);
        pointers.emplace_back(std::make_shared<TipVar>(n));
    }
    return pointers;
}

static void cleanup(std::vector<std::shared_ptr<TipType>> &values) {
    for(auto &value : values) {
        auto tipVar = std::dynamic_pointer_cast<TipVar>(value);
        delete tipVar->getNode();
    }
}

TEST_CASE("UnionFind: Test Constructor", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    auto tipVars = std::move(intsToTipVars(ints));
    UnionFind unionFind(tipVars);
    REQUIRE(true);

    cleanup(tipVars);
}

TEST_CASE("UnionFind: Test inserting nullptrs", "[UnionFind]") {
    std::vector<std::shared_ptr<TipType>> types {nullptr};
    REQUIRE_THROWS_AS(new UnionFind(types), std::invalid_argument);
}

TEST_CASE("UnionFind: Test find", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    auto tipVars = std::move(intsToTipVars(ints));

    auto three = tipVars.at(0);
    auto four = tipVars.at(1);
    auto five = tipVars.at(2);
    auto six = tipVars.at(3);
    auto seven = tipVars.at(4);
    auto eight = tipVars.at(5);
    auto nine = tipVars.at(6);

    UnionFind unionFind(tipVars);
    unionFind.quick_union(four, three);
    unionFind.quick_union(three, eight);
    unionFind.quick_union(nine, four);

    auto actualRootOfFour = std::dynamic_pointer_cast<TipVar>(unionFind.find(four));
    auto actualIntOfRootOfFour = dynamic_cast<ASTNumberExpr *>(actualRootOfFour->getNode())->getValue();
    auto actualRootOfNine = std::dynamic_pointer_cast<TipVar>(unionFind.find(nine));
    auto actualIntOfRootOfNine = dynamic_cast<ASTNumberExpr *>(actualRootOfNine->getNode())->getValue();

    REQUIRE(8 == actualIntOfRootOfFour);
    REQUIRE(8 == actualIntOfRootOfNine);
    cleanup(tipVars);
}

TEST_CASE("UnionFind: Test connected", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    auto tipVars = std::move(intsToTipVars(ints));

    auto three = tipVars.at(0);
    auto four = tipVars.at(1);
    auto five = tipVars.at(2);
    auto six = tipVars.at(3);
    auto seven = tipVars.at(4);
    auto eight = tipVars.at(5);
    auto nine = tipVars.at(6);

    UnionFind unionFind(tipVars);
    unionFind.quick_union(four, three);
    unionFind.quick_union(three, eight);
    unionFind.quick_union(nine, four);

    REQUIRE(unionFind.connected(three, nine));
    REQUIRE_FALSE(unionFind.connected(five, six));
    cleanup(tipVars);
}
