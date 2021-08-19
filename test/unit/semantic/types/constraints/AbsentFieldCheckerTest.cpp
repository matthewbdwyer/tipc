#include "ASTHelper.h"
#include "SymbolTable.h"
#include "TypeConstraintCollectVisitor.h"
#include "AbsentFieldChecker.h"
#include "SemanticError.h"
#include "ExceptionContainsWhat.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>

static void runtest(std::stringstream &program, bool expectPass) {
  auto ast = ASTHelper::build_ast(program);
  auto symbols = SymbolTable::build(ast.get());

  TypeConstraintCollectVisitor visitor(symbols.get());
  ast->accept(&visitor);

  auto unifier =  std::make_unique<Unifier>(visitor.getCollectedConstraints());
  unifier->solve();

  if (expectPass) {
    REQUIRE_NOTHROW(AbsentFieldChecker::check(ast.get(),unifier.get()));
  } else {
    REQUIRE_THROWS_MATCHES(AbsentFieldChecker::check(ast.get(),unifier.get()), 
                           SemanticError, 
                           ContainsWhat("absent field"));
  }
}


TEST_CASE("AbsentFieldChecker: record expr", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = {f: 4, g: 13};
          return 0;
      }
    )";

    runtest(program, true);
}

TEST_CASE("AbsentFieldChecker: direct read access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = {f: 4, g: 13};
          return r.g;
      }
    )";

    runtest(program, true);
}

TEST_CASE("AbsentFieldChecker: indirect read access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = alloc {f: 4, g: 13};
          return (*r).g;
      }
    )";

    runtest(program, true);
}


TEST_CASE("AbsentFieldChecker: direct write access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = {f: 4, g: 13};
          r.g = 17;
          return r.g;
      }
    )";

    runtest(program, true);
}

TEST_CASE("AbsentFieldChecker: indirect write access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      main() {
          var r;
          r = alloc {f: 4, g: 13};
          (*r).g = 17;
          return (*r).g;
      }
    )";

    runtest(program, true);
}

TEST_CASE("AbsentFieldChecker: record2", "[AbsentFieldChecker]") {
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

    runtest(program, true);
}

TEST_CASE("AbsentFieldChecker: record4", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
main() {
    var n, k, r1;
    k = {a: 1, b: 2};
    n = {c: &k, d: 4};
    r1 = ((*(n.c)).a); 
    output r1;
    return 0;
}

    )";

    runtest(program, true);
}

// The following are test that are expected to fail


TEST_CASE("AbsentFieldChecker: absent direct read access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      foo() {
          var r;
          r = {f: 4, g: 13};
          return r.h;
      }
    )";

    runtest(program, false);
}

TEST_CASE("AbsentFieldChecker: absent indirect read access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      foo() {
          var r;
          r = alloc {f: 4, g: 13};
          return (*r).h;
      }
    )";

    runtest(program, false);
}


TEST_CASE("AbsentFieldChecker: absent direct write access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      foo() {
          var r, q;
          q = {f: 1, g: 3};
          r = {f: 4, g: 13};
          r.h = q.h;
          return r.g;
      }
    )";

    runtest(program, false);
}

TEST_CASE("AbsentFieldChecker: absent indirect write access", "[AbsentFieldChecker]") {
    std::stringstream program;
    program << R"(
      foo() {
          var r, q;
          q = {f: 1, g: 3};
          r = alloc {f: 4, g: 13};
          (*r).h = q.h;
          return (*r).g;
      }
    )";

    runtest(program, false);
}
