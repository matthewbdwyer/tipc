#define CATCH_CONFIG_MAIN
#include "CGB.h"
#include "ASTHelper.h"
#include "SymbolTable.h"
#define EOF -1
#include <catch2/catch.hpp>

static void runtest(std::stringstream &program, int num_of_nodes, int num_of_edges) {

     auto ast = ASTHelper::build_ast(program);
     auto symTable = SymbolTable::build(ast.get());

     auto cfa = CFAnalyzer::analyze(ast.get(), symTable.get());
     auto call_graph = CallGraphBuilder::build(ast.get(), cfa);
     int total_edge = CallGraphBuilder::printCallGraph(ast -> getFunctions(), call_graph, std::cout);


     REQUIRE(ast -> getFunctions().size() == num_of_nodes); // functions are nodes
     REQUIRE(total_edge == num_of_edges); //there exists an edge (a1-> a0) if procedure a1 calls procedure a0.
     //call_graph.size() returns the total number of edges
     //perhaps we can do more checks, such as, if an edge exists or not
}

TEST_CASE("CallGraphBuilder: checks num of nodes and edges" "[CallGraphBuilder]") {
    std::stringstream program;
    program << R"(
      foo(x) {
        return x;
      }
      bar() {
        return foo(7);
      }
    )";
    runtest(program,2,1);
}


TEST_CASE("CallGraphBuilder: simple call graph" "[CallGraphBuilder]") {
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
    runtest(program,2,2);
}


TEST_CASE("CallGraphBuilder: more complicated call graph" "[CallGraphBuilder]") {
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
    runtest(program,4,6);
}