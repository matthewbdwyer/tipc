#include "ASTHelper.h"
#include "Stringifier.h"
#include "SymbolTable.h"
#include "TipAlpha.h"
#include "TipInt.h"
#include "TipRef.h"
#include "TypeConstraintVisitor.h"
#include "AppearingFieldsVisitor.h"
#include "catch.hpp"
#include <iostream>
#include <sstream>

static void printConstraints(std::vector<TypeConstraint> &constraints) {
    for(auto &tc : constraints) {
        std::cout << Stringifier::stringify(&tc) << std::endl;
    }
}

TEST_CASE("TypeConstraintVisitor: Test constraint generation", "[TypeConstraintVisitor]") {
    std::stringstream stream;
    stream << R"(
      short() {
        var x, y, z;
        x = input;
        y = alloc x;
        *y = x;
        z = *y;
        return z;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    std::stringstream outputStream;
    auto symbols = SymbolTable::build(ast.get(), outputStream);
    AppearingFieldsVisitor appearingFieldsVisitor;
    ast->accept(&appearingFieldsVisitor);
    auto fields = appearingFieldsVisitor.getFields();
    TypeConstraintVisitor typeConstraintVisitor(*symbols.value().get(), fields);
    ast->accept(&typeConstraintVisitor);

    std::vector<std::string> expectedConstraints {
            "[[input]] = int",
            "[[x]] = [[input]]",
            "[[alloc x]] = &[[x]]",
            "[[y]] = [[alloc x]]",
            "[[y]] = &[[x]]",
            "[[y]] = &[[*y]]",
            "[[z]] = [[*y]]",
            "[[short]] = () -> [[return z;]]"
    };

    auto tcs = typeConstraintVisitor.get_constraints();
    for(int i = 0; i < tcs.size(); i++) {
        auto expectedConstraint = expectedConstraints.at(i);
        auto actualConstraint = Stringifier::stringify(&tcs.at(i));
        REQUIRE(expectedConstraint == actualConstraint);
    }
    REQUIRE(tcs.size() == 8);
}

TEST_CASE("TypeConstraintVisitor: Test constraint generation2", "[TypeConstraintVisitor]") {
    std::stringstream stream;
    stream << R"(
      foo() {
        var x, y, z;
        x = 5;
        y = &y;
        z = foo;
        return z;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    std::stringstream outputStream;
    auto symbols = SymbolTable::build(ast.get(), outputStream);
    AppearingFieldsVisitor appearingFieldsVisitor;
    ast->accept(&appearingFieldsVisitor);
    auto fields = appearingFieldsVisitor.getFields();
    TypeConstraintVisitor typeConstraintVisitor(*symbols.value().get(), fields);
    ast->accept(&typeConstraintVisitor);

    std::vector<std::string> expectedConstraints {
            "[[5]] = int",
            "[[x]] = [[5]]",
            "[[&y]] = &[[y]]",
            "[[y]] = [[&y]]",
            "[[z]] = [[foo]]",
            "[[foo]] = () -> [[return z;]]"
    };

    auto tcs = typeConstraintVisitor.get_constraints();
    for(int i = 0; i < tcs.size(); i++) {
        auto expectedConstraint = expectedConstraints.at(i);
        auto actualConstraint = Stringifier::stringify(&tcs.at(i));
        REQUIRE(expectedConstraint == actualConstraint);
    }

    REQUIRE(tcs.size() == 6);
}