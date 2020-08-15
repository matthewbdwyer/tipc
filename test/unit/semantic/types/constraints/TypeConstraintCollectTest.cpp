#include "ASTHelper.h"
#include "Stringifier.h"
#include "SymbolTable.h"
#include "TypeConstraintCollectVisitor.h"
#include "catch.hpp"
#include <iostream>

static void printConstraints(std::vector<TypeConstraint> &constraints) {
    for(auto &tc : constraints) {
        std::cout << Stringifier::stringify(&tc) << std::endl;
    }
}

static void runtest(std::stringstream &program, std::vector<std::string> constraints) {
    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    TypeConstraintCollectVisitor visitor(symbols.get());
    ast->accept(&visitor);

    int i = 0;
    for (auto c : visitor.getCollectedConstraints()) {
      auto expected = constraints.at(i);
      auto actual = Stringifier::stringify(&c);
      REQUIRE(expected == actual);
      i++;
    }
    REQUIRE(visitor.getCollectedConstraints().size() == constraints.size());
}

TEST_CASE("TypeConstraintVisitor: const, input, alloc, assign through ptr", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      short() {
        var x, y, z;
        x = input;
        y = alloc x;
        *y = x;
        z = *y;
        return z;
      }
    )";

    std::vector<std::string> expected {
            "[[input]] = int",
            "[[x]] = [[input]]",
            "[[alloc x]] = &[[x]]",
            "[[y]] = [[alloc x]]",
            "[[y]] = &[[*y]]",
            "[[y]] = &[[x]]",
            "[[y]] = &[[*y]]",
            "[[z]] = [[*y]]",
            "[[short]] = () -> [[z]]"
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: function reference", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      foo() {
        var x, y, z;
        x = 5;
        y = &y;
        z = foo;
        return z;
      }
    )";

    std::vector<std::string> expected {
            "[[5]] = int",
            "[[x]] = [[5]]",
            "[[&y]] = &[[y]]",
            "[[y]] = [[&y]]",
            "[[z]] = [[foo]]",
            "[[foo]] = () -> [[z]]"
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: if ", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      foo() {
        var x;
        if (x > 0) {
          x = x + 1;
        }
        return x;
      }
    )";

    std::vector<std::string> expected {
            "[[0]] = int",          // const is int
            "[[x]] = [[0]]",		// operands have same type
            "[[(x > 0)]] = int",	// binexpr is int
            "[[x]] = [[(x > 0)]]", 	// operand and binexpr have same type
            "[[1]] = int",          // const is int
            "[[x]] = [[1]]",		// operands have same type
            "[[(x + 1)]] = int",	// type of binexpr
            "[[x]] = [[(x + 1)]]",	// operand and binexpr have same type
            "[[x]] = [[(x + 1)]]",	// sides of assignment have same type
            "[[(x > 0)]] = int",	// condition of if is int
            "[[foo]] = () -> [[x]]"
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: while ", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      foo() {
        var x;
        while (x > 0) {
          x = x - 1;
        }
        return x;
      }
    )";

    std::vector<std::string> expected {
            "[[0]] = int",          // const is int
            "[[x]] = [[0]]",		// operands have same type
            "[[(x > 0)]] = int",	// binexpr is int
            "[[x]] = [[(x > 0)]]", 	// operand and binexpr have same type
            "[[1]] = int",          // const is int
            "[[x]] = [[1]]",		// operands have same type
            "[[(x - 1)]] = int",	// type of binexpr
            "[[x]] = [[(x - 1)]]",	// operand and binexpr have same type
            "[[x]] = [[(x - 1)]]",	// sides of assignment have same type
            "[[(x > 0)]] = int",	// condition of while is int
            "[[foo]] = () -> [[x]]"
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: error, output", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      bar() {
        var x;
        x = input;
        output x;
        error x;
        return 0;
      }
    )";

    std::vector<std::string> expected {
            "[[input]] = int",    // input value is int
            "[[x]] = [[input]]",  // sides of assignment have same type
            "[[x]] = int",        // output arg is int
            "[[x]] = int",        // error arg is int
            "[[0]] = int",        // int constant
            "[[bar]] = () -> [[0]]"
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: funs with params", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      foo(x) {
        return x;
      }
      bar() {
        return foo(7);
      }
    )";

    std::vector<std::string> expected {
            "[[foo]] = ([[x]]) -> [[x]]",       // function with arg
            "[[7]] = int",                      // int constant
            "[[foo]] = ([[7]]) -> [[foo(7)]]",  // function application
            "[[bar]] = () -> [[foo(7)]]"
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: main", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      main(x) {
        return 0;
      }
    )";

    std::vector<std::string> expected {
      "[[0]] = int",                  // int constant in return
      "[[x]] = int",                  // main arg
      "[[0]] = int",                  //  main return
      "[[main]] = ([[x]]) -> [[0]]",  // main with arg
    };

    runtest(program, expected);
}


TEST_CASE("TypeConstraintVisitor: record expr", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = {f: 4, g: 13};
          return 0;
      }

    )";

    std::vector<std::string> expected {
      "[[4]] = int",				            // int constant
      "[[13]] = int",				            // int constant
      "[[{f:4, g:13}]] = {f:[[4]], g:[[13]]}",  // uber record
      "[[r]] = [[{f:4, g:13}]]",                // assignment
      "[[0]] = int",                            // int constant return
      "[[0]] = int",                            // main return
      "[[main]] = () -> [[0]]"                  // main declaration
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: access expr", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = {f: 4, g: 13};
          return r.g;
      }

    )";

    std::vector<std::string> expected {
      "[[4]] = int",                            // int constant
      "[[13]] = int",                           // int constant
      "[[{f:4, g:13}]] = {f:[[4]], g:[[13]]}",  // uber record
      "[[r]] = [[{f:4, g:13}]]",                // assignment
      "[[r]] = {f:\u03B1<f>, g:[[r.g]]}",       // field access
      "[[r.g]] = int",                          // main return
      "[[main]] = () -> [[r.g]]"                // main declaration
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: uber record", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      foo() {
          var r;
          r = {f: 4, g: 13};
          r = {n: null, f: 13};
          return 0;
      }

    )";

    std::vector<std::string> expected {
      "[[4]] = int",                                               // int constant
      "[[13]] = int",                                              // int constant
      "[[{f:4, g:13}]] = {f:[[4]], g:[[13]], n:\u03B1<n>}",        // uber record
      "[[r]] = [[{f:4, g:13}]]",                                   // assignment
      "[[null]] = &\u03B1<null>",                                  // null
      "[[13]] = int",                                              // int constant
      "[[{n:null, f:13}]] = {f:[[13]], g:\u03B1<g>, n:[[null]]}",  // uber record
      "[[r]] = [[{n:null, f:13}]]",                                // assignment
      "[[0]] = int",                                               // int constant
      "[[foo]] = () -> [[0]]"				                       // fun declaration
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: record2", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
main() {
    var n, r1;
    n = alloc {p: 4, q: 2};
    *n = {p:5, q: 6};
    r1 = (*n).p;
    output r1;
    return 0;
}
    )";

    std::vector<std::string> expected {
      "[[4]] = int",                                    // int constant
      "[[2]] = int",                                    // int constant
      "[[{p:4, q:2}]] = {p:[[4]], q:[[2]]}",        	// uber record
      "[[alloc {p:4, q:2}]] = &[[{p:4, q:2}]]",        	// uber record
      "[[n]] = [[alloc {p:4, q:2}]]",                   // assignment
      "[[n]] = &[[*n]]",				// deref
      "[[5]] = int",                                    // int constant
      "[[6]] = int",                                    // int constant
      "[[{p:5, q:6}]] = {p:[[5]], q:[[6]]}",        	// uber record
      "[[n]] = &[[{p:5, q:6}]]",			// assign through ptr
      "[[n]] = &[[*n]]",				// deref
      "[[*n]] = {p:[[*n.p]], q:\u03B1<q>}",       	// field access
      "[[r1]] = [[*n.p]]",				// assign 
      "[[r1]] = int",					// output
      "[[0]] = int",                                    // return int constant
      "[[0]] = int",                                    // int constant
      "[[main]] = () -> [[0]]"				// fun declaration
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: complex records", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      main() {
          var n, k, r1;
          k = {f: 4};
          n = alloc {p: 4, q: 5};
          *n = {p: 44, q: &k};
          return *(*n.q).f;
      }

    )";

    std::vector<std::string> expected {
      "[[4]] = int",                                           // int constant
      "[[{f:4}]] = {f:[[4]], p:\u03B1<p>, q:\u03B1<q>}",       // uber record
      "[[k]] = [[{f:4}]]",                                     // assignment
      "[[4]] = int",                                           // int constant
      "[[5]] = int",                                           // int constant
      "[[{p:4, q:5}]] = {f:\u03B1<f>, p:[[4]], q:[[5]]}",      // uber record
      "[[alloc {p:4, q:5}]] = &[[{p:4, q:5}]]",                // alloc is ref to init
      "[[n]] = [[alloc {p:4, q:5}]]",                          // assignment
      "[[n]] = &[[*n]]",				                       // deref
      "[[44]] = int",                                          // int constant
      "[[&k]] = &[[k]]",					                   // address of
      "[[{p:44, q:&k}]] = {f:\u03B1<f>, p:[[44]], q:[[&k]]}",  // uber record
      "[[n]] = &[[{p:44, q:&k}]]",				               // assign through ptr
      "[[n]] = {f:\u03B1<f>, p:\u03B1<p>, q:[[n.q]]}",		   // field access
      "[[n.q]] = &[[*n.q]]",					               // deref ptr
      "[[*n.q]] = {f:[[*n.q.f]], p:\u03B1<p>, q:\u03B1<q>}",   // field access
      "[[*n.q.f]] = &[[**n.q.f]]",                             // deref ptr
      "[[**n.q.f]] = int",                                     // main return value
      "[[main]] = () -> [[**n.q.f]]"                           // main declaration
    };

    runtest(program, expected);
}
