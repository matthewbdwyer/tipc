#define CATCH_CONFIG_MAIN
#include "CallGraph.h"
#include "ASTHelper.h"
#include "SymbolTable.h"
#define EOF -1
#include <catch2/catch.hpp>

TEST_CASE("CallGraph: getter for num of vertices and edges" "[CallGraph]") {
    std::stringstream program;
    program << R"(
      foo(x) {
        return x;
      }
      bar() {
        return foo(7);
      }
    )";

     auto ast = ASTHelper::build_ast(program);
     auto symTable = SymbolTable::build(ast.get());
     auto callGraph = CallGraph::build(ast.get(), symTable.get());

     REQUIRE(callGraph.get()->getTotalVertices() == 2); // 2 subroutines
     REQUIRE(callGraph.get()->getTotalEdges() == 1); // 1 edge from bar ->foo
}


TEST_CASE("CallGraph: simple call graph with cycle(recursion)" "[CallGraph]") {
    std::stringstream program;
    program << R"(
      foo(n,f) {
          var r;
          if (n==0) {
            r=1;
          } else {
            r = n * f(n-1,f);
          }
          return r;
      }

      main() {
          var n;
          n = 7;
          if (5040 != foo(n,foo)) error foo(n,foo);
          return 0;
      }
    )";

     auto ast = ASTHelper::build_ast(program);
     auto symTable = SymbolTable::build(ast.get());
     auto callGraph = CallGraph::build(ast.get(), symTable.get());

     REQUIRE(callGraph.get()->getTotalVertices() == 2); // 2 subroutines
     REQUIRE(callGraph.get()->getTotalEdges() == 2); // 1 edge from main ->foo and foo->foo


      //std::vector<std::pair<ASTFunction*, ASTFunction*>> edges =  callGraph.get()->getEdges();
      //check if there is an edge between foo and foo
      REQUIRE(true == callGraph.get()->existEdge("foo", "foo"));
}


TEST_CASE("CallGraph: test complex call graph (overapproximations)" "[CallGraph]") {
    std::stringstream program;
    program << R"(
      f() {
        return 3;
      }

      g() {
        var x;
        x = f(); // call function by name
        return x;
      }

      h(h1) {
        var y, r;
        y = h1;  // assign function reference
        r = y(); // call function by reference
        return r;
      }

      main() {
        if (h(f) != 3) error h(f);
        if (f() != 3) error f();
        if (h(g) != 3) error h(g);
        return 0;
      }
    )";

     auto ast = ASTHelper::build_ast(program);
     auto symTable = SymbolTable::build(ast.get());
     auto callGraph = CallGraph::build(ast.get(), symTable.get());

     //callGraph.get()->print(std::cout);

     REQUIRE(callGraph.get()->getTotalVertices() == 4); // 2 subroutines
     REQUIRE(callGraph.get()->getTotalEdges() == 6);

     //check overapproximations, e.g., edge from h->g and h->f
     REQUIRE(true == callGraph.get()->existEdge("h", "g"));
     REQUIRE(true == callGraph.get()->existEdge("h", "f"));
}