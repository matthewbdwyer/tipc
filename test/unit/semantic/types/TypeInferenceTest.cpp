#include "TypeConstraintCollectVisitor.h"
#include "ASTHelper.h"
#include "SymbolTable.h"
#include "Unifier.h"
#include "UnificationError.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>
#include <set>

// When we have a thorough set of tests at this level of abstraction
// we should remove the brittle tests below, e.g., the type constraint
// collector tests.  People can debug their type constraints when one
// of these fails they can look at the log.  Writeup a description of
// how to debug using the log messages.

/* There are two types of unit tests:
 *   1) those that check whether the program is type correct or not.
 *      These are classied as "TypeCheckTests".
 *   2) those that check that identifiers have the correct inferred type
 *      These are classied as "InferredTypeTests".
 */

static void typecheck(std::stringstream &program, bool expectedToPass) {
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

// TBD : need a thorough set of small focused type check tests

TEST_CASE("TypeCheckTests: arithmetic", "[TypeCheckTests]") {
    std::stringstream program;
    program << R"(add() {
       var x,y;
       return x+y;
    }
    )";

    typecheck(program, true);
}

TEST_CASE("TypeCheckTests: fail arithmetic", "[TypeCheckTests]") {
    std::stringstream program;
    program << R"(add() {
       var x,y;
       return x+add;
    }
    )";

    typecheck(program, false);
}

static void checkinferredtypes(std::stringstream &program, 
                               std::map<std::string, std::string> expected) {
    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    TypeConstraintCollectVisitor visitor(symbols.get());
    ast->accept(&visitor);

    auto unifier = std::make_unique<Unifier>(visitor.getCollectedConstraints());
    unifier->solve();

    std::map<std::string, ASTDeclNode*> funDeclMap;
   
    for(auto const& [name, expectedType] : expected) {
      ASTDeclNode* declNode = nullptr;
      if (name.find(':') != std::string::npos) {
        // match inferred parameter or local type
        auto funName = name.substr(0,name.find(':'));
        auto varName = name.substr(name.find(':')+1,name.length());
        declNode = symbols->getLocal(varName, funDeclMap[funName]);
      } else {
        // match inferred function type
        declNode = symbols->getFunction(name);
        funDeclMap[name] = declNode;
      }
      auto idVar = std::make_shared<TipVar>(declNode);
      auto idType = unifier->inferred(idVar);
      std::stringstream typeStream;
      typeStream << *idType;
      REQUIRE(expectedType == typeStream.str());
    }
}

TEST_CASE("InferredTypeTests: arithmetic", "[InferredTypeTests]") {
    std::stringstream program;
    program << R"(add() {
       var x,y;
       return x+y;
    }
    )";

    // The expected type information comes in the following format:
    // - function names precede parameters and locals for the function
    // - parameter and locals follow the function name with an intervening ":"
    // - with each name a string representation of a type is paired
    std::map<std::string, std::string> expected = {
      {"add", "() -> int" },
      {"add:x", "int"},
      {"add:y", "int"}
    };

    checkinferredtypes(program, expected);
}
