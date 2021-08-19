#include "TypeConstraintCollectVisitor.h"
#include "ASTHelper.h"
#include "SymbolTable.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>

static void runtest(std::stringstream &program, std::vector<std::string> constraints) {
    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    TypeConstraintCollectVisitor visitor(symbols.get());
    ast->accept(&visitor);

    auto collected = visitor.getCollectedConstraints();
    for(int i = 0; i < collected.size(); i++) {
        std::stringstream stream;
        stream << collected.at(i);
        auto actual = stream.str();
        auto expected = constraints.at(i);
        REQUIRE(expected == actual);
    }

    REQUIRE(visitor.getCollectedConstraints().size() == constraints.size());
}

TEST_CASE("TypeConstraintVisitor: const, input, alloc, assign through ptr", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(short() {
 var x,y,z;
 x = input;
 y = alloc x;
 *y = x;
 z = *y;
 return z;
}
    )";

    /*
     * For all of the test cases the expected results should be written with the following in mind:
     *   @l:c  lines and columns are numbered from 1 and 0, respectively
     *   ( )   non-trivial expressions are parenthesized
     *   order the order of constraints is determined by the post-order AST visit
     *   space there is no spacing between operators and their operands
     *   id    identifier expressions are mapped to their declaration
     *
     * Note that spacing is calculated immediately after the R"( in the program string literals.
     */
    std::vector<std::string> expected {
            "[[input@3:5]] = int",
            "[[x@2:5]] = [[input@3:5]]",
            "[[alloc x@4:5]] = &[[x@2:5]]",
            "[[y@2:7]] = [[alloc x@4:5]]",
            "[[y@2:7]] = &[[(*y)@5:1]]",
            "[[y@2:7]] = &[[x@2:5]]",
            "[[y@2:7]] = &[[(*y)@6:5]]",
            "[[z@2:9]] = [[(*y)@6:5]]",
            "[[short@1:0]] = () -> [[z@2:9]]"
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
            "[[5@4:12]] = int",
            "[[x@3:12]] = [[5@4:12]]",
            "[[&y@5:12]] = &[[y@3:15]]",
            "[[y@3:15]] = [[&y@5:12]]",
            "[[z@3:18]] = [[foo@2:6]]",
            "[[foo@2:6]] = () -> [[z@3:18]]"
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

    /*
     * These results do a good job of illustrating the redundancy in the constraints.
     * This arises because each expression generates its own constraints in isolation.
     */
    std::vector<std::string> expected {
      "[[0@4:16]] = int",       	// const is int
      "[[(x>0)@4:12]] = int",		// binexpr is int
      "[[x@3:12]] = int",		// operand is int
      "[[0@4:16]] = int", 		// operand is int
      "[[1@5:18]] = int",       	// const is int
      "[[(x+1)@5:14]] = int",		// binexpr is int
      "[[x@3:12]] = int",		// operands is int
      "[[1@5:18]] = int",		// operands is int
      "[[x@3:12]] = [[(x+1)@5:14]]",	// sides of assignment have same type
      "[[(x>0)@4:12]] = int",		// if condition is int
      "[[foo@2:6]] = () -> [[x@3:12]]"  // function type
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
      "[[0@4:19]] = int",       	// const is int
      "[[(x>0)@4:15]] = int",		// binexpr is int
      "[[x@3:12]] = int",		// operand is int
      "[[0@4:19]] = int", 		// operand is int
      "[[1@5:18]] = int",       	// const is int
      "[[(x-1)@5:14]] = int",		// binexpr is int
      "[[x@3:12]] = int",		// operands is int
      "[[1@5:18]] = int",		// operands is int
      "[[x@3:12]] = [[(x-1)@5:14]]",	// sides of assignment have same type
      "[[(x>0)@4:15]] = int",		// while condition is int
      "[[foo@2:6]] = () -> [[x@3:12]]"  // function type
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
      "[[input@4:12]] = int",    	// input value is int
      "[[x@3:12]] = [[input@4:12]]",  	// sides of assignment have same type
      "[[x@3:12]] = int",        	// output arg is int
      "[[x@3:12]] = int",        	// error arg is int
      "[[0@7:15]] = int",        	// int constant
      "[[bar@2:6]] = () -> [[0@7:15]]"	// function type
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
      "[[foo@2:6]] = ([[x@2:10]]) -> [[x@2:10]]",	// function with arg
      "[[7@6:19]] = int",				// int constant
      "[[foo@2:6]] = ([[7@6:19]]) -> [[foo(7)@6:15]]",	// function application
      "[[bar@5:6]] = () -> [[foo(7)@6:15]]"		// function no arg
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
      "[[0@3:15]] = int",                  		// int constant 
      "[[x@2:11]] = int",                  		// main args are int 
      "[[0@3:15]] = int",                  		// main return is int
      "[[main@2:6]] = ([[x@2:11]]) -> [[0@3:15]]"  	// function with arg
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: poly", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      poly(p) {
        return *p;
      }
    )";

    std::vector<std::string> expected {
      "[[p@2:11]] = &[[(*p)@3:15]]",              	// deref
      "[[poly@2:6]] = ([[p@2:11]]) -> [[(*p)@3:15]]", 	// function with param
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
      "[[4@4:18]] = int",				        // int constant
      "[[13@4:24]] = int",				        // int constant
      "[[{f:4,g:13}@4:14]] = {f:[[4@4:18]],g:[[13@4:24]]}",	// uber record
      "[[r@3:14]] = [[{f:4,g:13}@4:14]]",			// assignment
      "[[0@5:17]] = int",					// int constant
      "[[0@5:17]] = int",					// main return is int
      "[[main@2:6]] = () -> [[0@5:17]]"				// function decl
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
      "[[4@4:18]] = int",                            		// int constant
      "[[13@4:24]] = int",                           		// int constant
      "[[{f:4,g:13}@4:14]] = {f:[[4@4:18]],g:[[13@4:24]]}",	// uber record
      "[[r@3:14]] = [[{f:4,g:13}@4:14]]",                	// assignment
      "[[r@3:14]] = {f:\u03B1<(r.g):f>,g:[[(r.g)@5:17]]}",      // field access
      "[[(r.g)@5:17]] = int",                          		// main return int
      "[[main@2:6]] = () -> [[(r.g)@5:17]]"                	// function decl
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
      "[[4@4:18]] = int",                            		// int constant
      "[[13@4:24]] = int",                           		// int constant
      "[[{f:4,g:13}@4:14]] = {f:[[4@4:18]],g:[[13@4:24]],n:\u25C7}",	// uber record
      "[[r@3:14]] = [[{f:4,g:13}@4:14]]",                	// assignment
      "[[null@5:18]] = &\u03B1<null>",                		// null
      "[[13@5:27]] = int",                           		// int constant
      "[[{n:null,f:13}@5:14]] = {f:[[13@5:27]],g:\u25C7,n:[[null@5:18]]}",	// uber record
      "[[r@3:14]] = [[{n:null,f:13}@5:14]]",      		// assignment
      "[[0@6:17]] = int",                          		// main return int
      "[[foo@2:6]] = () -> [[0@6:17]]"                		// function decl
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
      "[[4@4:18]] = int",                                 // int constant
      "[[2@4:24]] = int",                                 // int constant
      "[[{p:4,q:2}@4:14]] = {p:[[4@4:18]],q:[[2@4:24]]}", // record
      "[[alloc {p:4,q:2}@4:8]] = &[[{p:4,q:2}@4:14]]",    // alloc ptr
      "[[n@3:8]] = [[alloc {p:4,q:2}@4:8]]",              // assignment
      "[[n@3:8]] = &[[(*n)@5:4]]",			  // deref
      "[[5@5:12]] = int",                                 // int constant
      "[[6@5:18]] = int",                                 // int constant
      "[[{p:5,q:6}@5:9]] = {p:[[5@5:12]],q:[[6@5:18]]}",  // record
      "[[n@3:8]] = &[[{p:5,q:6}@5:9]]",                   // assign through ptr
      "[[n@3:8]] = &[[(*n)@6:10]]",			  // deref
      "[[(*n)@6:10]] = {p:[[((*n).p)@6:9]],q:\u03B1<((*n).p):q>}",       // field access
      "[[r1@3:11]] = [[((*n).p)@6:9]]",			  // assign 
      "[[r1@3:11]] = int",				  // output
      "[[0@8:11]] = int",				  // main return int
      "[[0@8:11]] = int",				  // int constant
      "[[main@2:0]] = () -> [[0@8:11]]"			  // fun declaration
    };

    runtest(program, expected);
}

TEST_CASE("TypeConstraintVisitor: record4", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
main() {
    var n, k, r1;
    k = {a: 1, b: 2};
    n = {c: &k, d: 4};
    r1 = ((*(n.c)).a); // output 1
    output r1;
    return 0;
}

    )";

    std::vector<std::string> expected {
      "[[1@4:12]] = int",                                    // int constant
      "[[2@4:18]] = int",                                    // int constant
      "[[{a:1,b:2}@4:8]] = {a:[[1@4:12]],b:[[2@4:18]],c:\u25C7,d:\u25C7}", // record
      "[[k@3:11]] = [[{a:1,b:2}@4:8]]",		     	     // assign
      "[[&k@5:12]] = &[[k@3:11]]",                           // address of
      "[[4@5:19]] = int",                                    // int constant
      "[[{c:&k,d:4}@5:8]] = {a:\u25C7,b:\u25C7,c:[[&k@5:12]],d:[[4@5:19]]}", // record
      "[[n@3:8]] = [[{c:&k,d:4}@5:8]]",		     	     // assign
      "[[n@3:8]] = {a:\u03B1<(n.c):a>,b:\u03B1<(n.c):b>,c:[[(n.c)@6:13]],d:\u03B1<(n.c):d>}", // access
      "[[(n.c)@6:13]] = &[[(*(n.c))@6:11]]",		     // assign through ptr
      "[[(*(n.c))@6:11]] = {a:[[((*(n.c)).a)@6:10]],b:\u03B1<((*(n.c)).a):b>,c:\u03B1<((*(n.c)).a):c>,d:\u03B1<((*(n.c)).a):d>}", // access
      "[[r1@3:14]] = [[((*(n.c)).a)@6:10]]",	  	     // assign 
      "[[r1@3:14]] = int",				     // output
      "[[0@8:11]] = int",                                    // main return int
      "[[0@8:11]] = int",                                    // int constant
      "[[main@2:0]] = () -> [[0@8:11]]"			     // fun declaration
    };

    runtest(program, expected);
}
