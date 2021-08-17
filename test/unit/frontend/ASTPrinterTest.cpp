#include "ASTHelper.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("ASTPrinterTest: output test", "[ASTNodePrint]") {
    std::stringstream stream;
    stream << R"(
      foo() { output 42; return 0; }
    )";

    std::vector<std::string> expected {
      "output 42;",
      "return 0;"
    };

    auto ast = ASTHelper::build_ast(stream);

    auto f = ast->findFunctionByName("foo");

    int i = 0;
    for (auto s : f->getStmts()) {
      stream = std::stringstream();
      stream << *s;
      auto actual = stream.str();
      REQUIRE(actual == expected.at(i++));
    }

}

TEST_CASE("ASTPrinterTest: function printers", "[ASTNodePrint]") {
    std::stringstream stream;
    stream << R"(
      fun(a) { return a; }
      foo() { return 1; }
      bar(x, y) { return x+1; }
    )";

    std::vector<std::string> expected {
      "fun(a) {...}",
      "foo() {...}",
      "bar(x,y) {...}"
    };

    auto ast = ASTHelper::build_ast(stream);

    int i = 0;
    for (auto f : ast->getFunctions()) {
      stream = std::stringstream();
      stream << *f;
      auto actual = stream.str();
      REQUIRE(actual == expected.at(i++));
    }
}

TEST_CASE("ASTPrinterTest: statement printers", "[ASTNodePrint]") {
    std::stringstream stream;
    stream << R"(
      fun() {
        var x, /* comment */ y, z;
        x = 
            0;
        if (x == 0) 
          y = 0;
        if (y == z) y=1; else y    =    2;
        while (x == 0) 
          z = 0;
        { 
           x = y + z; 
           y = 13;
        } 
        error       z;
        return z;
      }
    )";

    std::vector<std::string> expected {
      "var x, y, z;",
      "x = 0;",
      "if ((x==0)) y = 0;",
      "if ((y==z)) y = 1; else y = 2;",
      "while ((x==0)) z = 0;",
      "{ x = (y+z); y = 13; }",
      "error z;",
      "return z;"
    };

    auto ast = ASTHelper::build_ast(stream);

    auto f = ast->findFunctionByName("fun");

    int i = 0;
    for (auto d : f->getDeclarations()) {
      stream = std::stringstream();
      stream << *d;
      auto actual = stream.str();
      REQUIRE(actual == expected.at(i++));
    }

    for (auto s : f->getStmts()) {
      stream = std::stringstream();
      stream << *s;
      auto actual = stream.str();
      REQUIRE(actual == expected.at(i++));
    }

}

TEST_CASE("ASTPrinterTest: expression printers", "[ASTNodePrint]") {
    std::stringstream stream;
    stream << R"(
      foo(a) { return a;}
      fun() {
        var x, y, z;
        x = y + 0;
        y = input;
        z = {next:null, val:42};
        y = z.val;
        x = (*z).next;
        x = &z;
        z = -3;
        z = (42);
        x = alloc null;
        y = x + y - z * 3 / 1;
        y = foo(x);
        return 0;
      }
    )";

    std::vector<std::string> expected {
      "(y+0)",
      "input",
      "{next:null,val:42}",
      "(z.val)",
      "((*z).next)",
      "&z",
      "-3",
      "42",
      "alloc null",
      "((x+y)-((z*3)/1))",
      "foo(x)"
    };

    auto ast = ASTHelper::build_ast(stream);

    auto f = ast->findFunctionByName("fun");

    int i = 0;
    int numStmts = f->getStmts().size() - 1;  // skip the return
    for (auto s : f->getStmts()) {
      auto a = dynamic_cast<ASTAssignStmt*>(s);
      stream = std::stringstream();
      stream << *a->getRHS();
      auto actual = stream.str();
      REQUIRE(actual == expected.at(i++));
      if (i == numStmts) break;
    }
}

TEST_CASE("ASTPrinterTest: conditional expression printers", "[ASTNodePrint]") {
    std::stringstream stream;
    stream << R"(
      fun() {
        var x, y, z;
        if (x == y) x = 0;
        if (x != y) x = 0;
        if (x > y) x = 0;
        return 0;
      }
    )";

    std::vector<std::string> expected {
      "(x==y)",
      "(x!=y)",
      "(x>y)"
    };

    auto ast = ASTHelper::build_ast(stream);

    auto f = ast->findFunctionByName("fun");

    int i = 0;
    int numStmts = f->getStmts().size() - 1;  // skip the return
    for (auto s : f->getStmts()) {
      auto ifstmt = dynamic_cast<ASTIfStmt*>(s);
      stream = std::stringstream();
      stream << *ifstmt->getCondition();
      auto actual = stream.str();
      REQUIRE(actual == expected.at(i++));
      if (i == numStmts) break;
    }
}

TEST_CASE("ASTPrinterTest: local expr test", "[ASTNodePrint]") {
    // Constructor expects unique pointers so we must create them
    auto zero = std::make_unique<ASTNumberExpr>(0); 
    auto var = std::make_unique<ASTVariableExpr>("y"); 

    // Here we just use the default constructor
    ASTBinaryExpr ypluszero("+", std::move(var), std::move(zero)); 

    std::stringstream  stream;
    stream << ypluszero;
    auto actual = stream.str();

    REQUIRE(actual == "(y+0)");
}

TEST_CASE("ASTPrinterTest: local unique expr test", "[ASTNodePrint]") {
    auto zero = std::make_unique<ASTNumberExpr>(0); 
    auto var = std::make_unique<ASTVariableExpr>("y"); 

    // Here we create a unique pointer to the binary expr
    auto ypluszero = std::make_unique<ASTBinaryExpr>("+", std::move(var), std::move(zero)); 

    std::stringstream  stream;
    stream << *ypluszero; // dereference is an operation for unique pointers
    auto actual = stream.str();

    REQUIRE(actual == "(y+0)");
}

TEST_CASE("ASTPrinterTest: ASTProgram output is the hash of the source.", "[ASTNodePrint]") {
  std::stringstream stream;
  stream << R"(
      foo(x) {
         return x + 1;
      }

      main() {
        var z;
        z = foo(42);
        return z;
      }
    )";

  auto ast = ASTHelper::build_ast(stream);

  std::string expectedOutput = "29e272ab0b8752287594fb0014953a9cf7879e3687f4d481ced352a835b46359";
  std::stringstream actualOutput;
  actualOutput << *ast;
  REQUIRE(expectedOutput == actualOutput.str());
}
