#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "UnionFind.h"
#include <vector>

TEST_CASE("UnionFind: Test Constructor", "[UnionFind]") {
    SECTION("Templated int") {
        std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
        UnionFind<int> unionFindInt(ints);
        REQUIRE(true);
    }

    SECTION("Templated long") {
        std::vector<long> longs {3l, 4l, 5l, 6l, 7l, 8l, 9l};
        UnionFind<long> unionFindLong(longs);
        REQUIRE(true);
    }

    SECTION("Templated string") {
        std::vector<std::string> strings {"foo"};
        UnionFind<std::string> unionFindStrings(strings);
        REQUIRE(true);
    }

}

TEST_CASE("UnionFind: Test find", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    UnionFind<int> unionFind(ints);
    unionFind.quick_union(4, 3);
    unionFind.quick_union(3, 8);
    unionFind.quick_union(9, 4);

    REQUIRE(8 == unionFind.find(4));
    REQUIRE(8 == unionFind.find(9));
}

TEST_CASE("UnionFind: Test connected", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    UnionFind<int> unionFind(ints);
    unionFind.quick_union(4, 3);
    unionFind.quick_union(3, 8);
    unionFind.quick_union(9, 4);

    REQUIRE(unionFind.connected(3, 9));
    REQUIRE_FALSE(unionFind.connected(5, 6));
}
