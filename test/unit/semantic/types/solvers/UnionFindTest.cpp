#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "UnionFind.h"
#include <vector>
#include <utility>

template<typename T>
static std::vector<T*> toPointers(std::vector<T> &values) {
    std::vector<T*> pointers;
    for(auto &value : values) {
        pointers.push_back(&value);
    }
    return pointers;
}

TEST_CASE("UnionFind: Test Constructor", "[UnionFind]") {
    SECTION("Templated int") {
        std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
        UnionFind<int> unionFindInt(std::move(toPointers(ints)));
        REQUIRE(true);
    }

    SECTION("Templated long") {
        std::vector<long> longs {3l, 4l, 5l, 6l, 7l, 8l, 9l};
        UnionFind<long> unionFindLong(std::move(toPointers(longs)));
        REQUIRE(true);
    }

    SECTION("Templated string") {
        std::vector<std::string> strings {"foo"};
        UnionFind<std::string> unionFindStrings(std::move(toPointers(strings)));
        REQUIRE(true);
    }

}

TEST_CASE("UnionFind: Test find", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    auto intPointers = std::move(toPointers(ints));
    auto three = intPointers.at(0);
    auto four = intPointers.at(1);
    auto five = intPointers.at(2);
    auto six = intPointers.at(3);
    auto seven = intPointers.at(4);
    auto eight = intPointers.at(5);
    auto nine = intPointers.at(6);

    UnionFind<int> unionFind(intPointers);
    unionFind.quick_union(four, three);
    unionFind.quick_union(three, eight);
    unionFind.quick_union(nine, four);

    REQUIRE(8 == *unionFind.find(four));
    REQUIRE(8 == *unionFind.find(nine));
}

TEST_CASE("UnionFind: Test connected", "[UnionFind]") {
    std::vector<int> ints {3, 4, 5, 6, 7, 8, 9};
    auto intPointers = std::move(toPointers(ints));
    auto three = intPointers.at(0);
    auto four = intPointers.at(1);
    auto five = intPointers.at(2);
    auto six = intPointers.at(3);
    auto seven = intPointers.at(4);
    auto eight = intPointers.at(5);
    auto nine = intPointers.at(6);

    UnionFind<int> unionFind(intPointers);
    unionFind.quick_union(four, three);
    unionFind.quick_union(three, eight);
    unionFind.quick_union(nine, four);

    REQUIRE(unionFind.connected(three, nine));
    REQUIRE_FALSE(unionFind.connected(five, six));
}