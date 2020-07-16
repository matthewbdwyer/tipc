#include "catch.hpp"
#include "ASTHelper.h"
#include "SymbolTable.h"

#include <iostream>
#include <optional>

TEST_CASE("Symbol Table: locals", "[Symbol]") {
    std::stringstream stream;
    stream << R"(short() { var x, y, z; output x+y; return z; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    // There should be no error messages emitted
    REQUIRE(symbols != std::nullopt);

    SymbolTable::print(symbols.value().get(), outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {short}");
    REQUIRE(found!=std::string::npos);

    found = output.find("short : {x, y, z}");
    REQUIRE(found!=std::string::npos);
}


TEST_CASE("Symbol Table: functions", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo() { return 0; } bar() { return 1; } baz() { return 2;})";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    // There should be no error messages emitted
    REQUIRE(symbols != std::nullopt);

    SymbolTable::print(symbols.value().get(), outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {bar, baz, foo}");
    REQUIRE(found!=std::string::npos);

    found = output.find("foo : {}");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: params", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo(a) { return a; } bar(a) { return a; } baz(b) { return b;})";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    // There should be no error messages emitted
    REQUIRE(symbols != std::nullopt);

    SymbolTable::print(symbols.value().get(), outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {bar, baz, foo}");
    REQUIRE(found!=std::string::npos);

    found = output.find("foo : {a}");
    REQUIRE(found!=std::string::npos);

    found = output.find("bar : {a}");
    REQUIRE(found!=std::string::npos);

    found = output.find("baz : {b}");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: locals params ", "[Symbol]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x, y, z; return z; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    // There should be no error messages emitted
    REQUIRE(symbols != std::nullopt);

    SymbolTable::print(symbols.value().get(), outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {short}");
    REQUIRE(found!=std::string::npos);

    found = output.find("short : {a, b, c, x, y, z}");
    REQUIRE(found!=std::string::npos);
}

/******************** symbol errors *******************/

TEST_CASE("Symbol Table: local undeclared", "[Symbol]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x; return z; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    REQUIRE(symbols == std::nullopt);

    std::string output = outputStream.str();

    // There should be an error messages emitted
    std::size_t found = output.find("variable z is undeclared");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: locals param clash ", "[Symbol]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x, b, z; return z; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    REQUIRE(symbols == std::nullopt);

    std::string output = outputStream.str();

    // There should be an error messages emitted
    std::size_t found = output.find("local name b redeclared");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: locals clash ", "[Symbol]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x, x, z; return z; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    REQUIRE(symbols == std::nullopt);

    std::string output = outputStream.str();

    // There should be an error messages emitted
    std::size_t found = output.find("local name x redeclared");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: params clash ", "[Symbol]") {
    std::stringstream stream;
    stream << R"(short(a, b, a) { var x, y, z; return z; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    REQUIRE(symbols == std::nullopt);

    std::string output = outputStream.str();

    // There should be an error messages emitted
    std::size_t found = output.find("local name a redeclared");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: functions clash ", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo() { return 0; } bar() { return 1; } foo() { return 2;})";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto symbols = SymbolTable::build(ast.get(), outputStream);
    REQUIRE(symbols == std::nullopt);

    std::string output = outputStream.str();

    // There should be an error messages emitted
    std::size_t found = output.find("function name foo already declared");
    REQUIRE(found!=std::string::npos);
}
