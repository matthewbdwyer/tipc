#include "ASTHelper.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("ASTAccessExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
  std::stringstream stream;
  stream << R"(
      foo() {
         return {f : 0}.f;
      }
    )";

  auto ast = ASTHelper::build_ast(stream);
  auto expr = ASTHelper::find_node<ASTAccessExpr>(ast);

  std::stringstream o1;
  o1 << expr->getField();
  REQUIRE(o1.str() == "f");

  std::stringstream o2;
  o2 << *expr->getRecord();
  REQUIRE(o2.str() == "{f:0}");
}

TEST_CASE("ASTAllocExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return alloc 2 + 3;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTAllocExpr>(ast);

    std::stringstream o1;
    o1 << *expr->getInitializer();
    REQUIRE(o1.str() == "(2+3)");
}

TEST_CASE("ASTAssignStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(p) {
         *p = 2 + 7;
         return *p - 1;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTAssignStmt>(ast);

    std::stringstream o1;
    o1 << *stmt->getLHS();
    REQUIRE(o1.str() == "(*p)");

    std::stringstream o2;
    o2 << *stmt->getRHS();
    REQUIRE(o2.str() == "(2+7)");
}

TEST_CASE("ASTBinaryExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         var x;
         x = x + foo();
         return x;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTBinaryExpr>(ast);

    std::stringstream o1;
    o1 << *expr->getLeft();
    REQUIRE(o1.str() == "x");

    std::stringstream o2;
    o2 << *expr->getRight();
    REQUIRE(o2.str() == "foo()");

    std::stringstream o3;
    o3 << expr->getOp();
    REQUIRE(o3.str() == "+");
}

TEST_CASE("ASTBlockStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         var x;
         if (1 > 0) {
             x = 0;
             x = 1;
         }
         return x+1;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTBlockStmt>(ast);

    auto stmts = stmt->getStmts();
    REQUIRE(stmts.size() == 2);
}

TEST_CASE("ASTDeclNodeTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         var x;
         return 0;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto decl = ASTHelper::find_node<ASTDeclNode>(ast);

    REQUIRE(decl->getName() == "x");
}

TEST_CASE("ASTDeclStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         var v1, v2, v3, v4;
         return 0;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTDeclStmt>(ast);

    auto stmts = stmt->getVars();
    REQUIRE(stmts.size() == 4);
}

TEST_CASE("ASTDerefExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(p) {
         return **p;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTDeRefExpr>(ast);

    std::stringstream o1;
    o1 << *expr->getPtr();
    REQUIRE(o1.str() == "(*p)");
}

TEST_CASE("ASTErrorStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         error 13 - 1;
         return 0;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTErrorStmt>(ast);

    std::stringstream o1;
    o1 << *stmt->getArg();
    REQUIRE(o1.str() == "(13-1)");
}

TEST_CASE("ASTFieldExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return {f : 13};
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTFieldExpr>(ast);

    std::stringstream o1;
    o1 << expr->getField();
    REQUIRE(o1.str() == "f");

    std::stringstream o2;
    o2 << *expr->getInitializer();
    REQUIRE(o2.str() == "13");
}

TEST_CASE("ASTFunAppExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return bar(1,2,3);
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTFunAppExpr>(ast);

    std::stringstream o1;
    o1 << *expr->getFunction();
    REQUIRE(o1.str() == "bar");

    auto arguments = expr->getActuals();
    REQUIRE(arguments.size() == 3);
}

TEST_CASE("ASTFunctionTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(x, y) {
         var z;
         var r;
         z = x - 1;
         z = z * 2;
         return x + y;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto fun = ASTHelper::find_node<ASTFunction>(ast);

    std::stringstream o1;
    o1 << *fun->getDecl();
    REQUIRE(o1.str() == "foo");

    REQUIRE(fun->getName() == "foo");
    REQUIRE(!fun->isPoly());
    REQUIRE(fun->getFormals().size() == 2);
    REQUIRE(fun->getDeclarations().size() == 2);
    REQUIRE(fun->getStmts().size() == 3);
}

TEST_CASE("ASTIfStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(c) {
         var x;
         if (c > 0)
            x = 13;
         else
            x = 7;
         return x;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTIfStmt>(ast);

    std::stringstream o1;
    o1 << *stmt->getCondition();
    REQUIRE(o1.str() == "(c>0)");

    std::stringstream o2;
    o2 << *stmt->getThen();
    REQUIRE(o2.str() == "x = 13;");

    std::stringstream o3;
    o3 << *stmt->getElse();
    REQUIRE(o3.str() == "x = 7;");
}

TEST_CASE("ASTIfStmtTest: No else.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(c) {
         var x;
         x = 7;
         if (c > 0) x = 13;
         return x;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTIfStmt>(ast);

    REQUIRE(stmt->getElse() == nullptr);
}

TEST_CASE("ASTInputExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return input;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTInputExpr>(ast);

    REQUIRE(expr != nullptr);
}

TEST_CASE("ASTNullExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return null;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTNullExpr>(ast);

    REQUIRE(expr != nullptr);
}

TEST_CASE("ASTNumberExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return 13;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTNumberExpr>(ast);

    REQUIRE(expr->getValue() == 13);
}

TEST_CASE("ASTOutputStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         output 17;
         return 0;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTOutputStmt>(ast);

    std::stringstream o1;
    o1 << *stmt->getArg();
    REQUIRE(o1.str() == "17");
}

TEST_CASE("ASTRecordExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return {f : 0, g : 1, h : 2};
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTRecordExpr>(ast);

    REQUIRE(expr->getFields().size() == 3);
}

TEST_CASE("ASTRefExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         var x;
         return &x;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTRefExpr>(ast);

    std::stringstream o1;
    o1 << *expr->getVar();
    REQUIRE(o1.str() == "x");
}

TEST_CASE("ASTReturnStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo() {
         return 123;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTReturnStmt>(ast);

    std::stringstream o1;
    o1 << *stmt->getArg();
    REQUIRE(o1.str() == "123");
}

TEST_CASE("ASTVariableExprTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(x) {
         return x + 1;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto expr = ASTHelper::find_node<ASTVariableExpr>(ast);

    REQUIRE(expr->getName() == "x");
}

TEST_CASE("ASTWhileStmtTest: Test methods of AST subtype.",
          "[ASTNodes]") {
    std::stringstream stream;
    stream << R"(
      foo(x) {
         var y;
         while (x > 0) {
            x = x - 1;
         }
         return {f : 0}.f;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    auto stmt = ASTHelper::find_node<ASTWhileStmt>(ast);

    std::stringstream o1;
    o1 << *stmt->getCondition();
    REQUIRE(o1.str() == "(x>0)");

    std::stringstream o2;
    o2 << *stmt->getBody();
    REQUIRE(o2.str() == "{ x = (x-1); }");
}
