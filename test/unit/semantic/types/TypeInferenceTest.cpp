#include "TypeConstraintCollectVisitor.h"
#include "ASTHelper.h"
#include "SymbolTable.h"
#include "Unifier.h"
#include "UnificationError.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>
#include <set>

static void runtest(std::stringstream &program, bool expectedToPass) {
    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    TypeConstraintCollectVisitor visitor(symbols.get());
    ast->accept(&visitor);

    auto unifier = std::make_unique<Unifier>(visitor.getCollectedConstraints());

    if (expectedToPass) {
      REQUIRE_NOTHROW(unifier->solve());
    } else {
      REQUIRE_THROWS_AS(unifier->solve(), UnificationError);
    }
}

TEST_CASE("TypeInferenceTest: arithmetic", "[TypeInferenceTest]") {
    std::stringstream program;
    program << R"(add() {
       var x,y;
       return x+y;
    }
    )";

    runtest(program, true);
}

TEST_CASE("TypeInferenceTest: fail arithmetic", "[TypeInferenceTest]") {
    std::stringstream program;
    program << R"(add() {
       var x,y;
       return x+add;
    }
    )";

    runtest(program, false);
}

