#include "CallGraph.h"
#include "ASTHelper.h"
#include "SemanticAnalysis.h"
#include "SymbolTable.h"

#include <catch2/catch_test_macros.hpp>

#include <set>

TEST_CASE("CallGraph: getter for num of vertices and edges"
          "[CallGraph]") {
  std::stringstream program;
  program << R"(
      foo(x) {
        return x;
      }
      bar() {
        return foo(7);
      }
    )";

  /* Call graph should be:
   *   bar->foo
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  REQUIRE(callGraph.get()->getTotalVertices() == 2);
  REQUIRE(callGraph.get()->getTotalEdges() == 1);

  REQUIRE(callGraph.get()->existEdge("bar", "foo"));
}

TEST_CASE("CallGraph: simple call graph with cycle(recursion)"
          "[CallGraph]") {
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

  /* Call graph should be:
   *   main->foo	direct call
   *   foo->foo	indirect call through 2nd parameter
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  REQUIRE(callGraph.get()->getTotalVertices() == 2);
  REQUIRE(callGraph.get()->getTotalEdges() == 2);

  REQUIRE(callGraph.get()->existEdge("main", "foo"));
  REQUIRE(callGraph.get()->existEdge("foo", "foo"));
}

TEST_CASE("CallGraph: test complex call graph (overapproximations)"
          "[CallGraph]") {
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

  /* Call graph should be:
   *   main->h	direct call
   *   main->f	direct call
   *   h->g		indirect call through local from parameter
   *   h->f		indirect call through local from parameter
   *   g->f		direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  REQUIRE(callGraph.get()->getTotalVertices() == 4);
  REQUIRE(callGraph.get()->getTotalEdges() == 5);

  REQUIRE(callGraph.get()->existEdge("main", "h"));
  REQUIRE(callGraph.get()->existEdge("main", "f"));
  REQUIRE(callGraph.get()->existEdge("h", "g"));
  REQUIRE(callGraph.get()->existEdge("h", "f"));
  REQUIRE(callGraph.get()->existEdge("g", "f"));

  REQUIRE_FALSE(callGraph.get()->existEdge("f", "f"));
}

TEST_CASE("CallGraph: test getCallee by function name"
          "[CallGraph]") {
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

  /* Call graph should be:
   *   main->h	direct call
   *   main->f	direct call
   *   h->g		indirect call through local from parameter
   *   h->f		indirect call through local from parameter
   *   g->f		direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  REQUIRE(callGraph.get()->getCallees("main").size() == 2);
  REQUIRE(callGraph.get()->getCallees("h").size() == 2);
  REQUIRE(callGraph.get()->getCallees("g").size() == 1);
  REQUIRE(callGraph.get()->getCallees("f").size() == 0);
}

TEST_CASE("CallGraph: test getCallee by ASTFunction*"
          "[CallGraph]") {
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

  /* Call graph should be:
   *   main->h	direct call
   *   main->f	direct call
   *   h->g		indirect call through local from parameter
   *   h->f		indirect call through local from parameter
   *   g->f		direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  ASTFunction *caller = callGraph.get()->getASTFun("main");
  ASTFunction *callee = callGraph.get()->getASTFun("h");

  std::set<ASTFunction *> callees = callGraph.get()->getCallees(caller);
  REQUIRE(callees.size() == 2);

  // h is one of main's callees
  REQUIRE(callees.find(callee) != callees.end());
}

TEST_CASE("CallGraph: test getCallers"
          "[CallGraph]") {
  std::stringstream program;
  program << R"(
      foo(x) {
        return x;
      }
      bar() {
        return foo(7);
      }
    )";

  /* Call graph should be:
   *   bar->foo	direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  std::set<std::string> callers = callGraph.get()->getCallers("foo");
  REQUIRE(callers.size() == 1);

  // bar is one of foo's callers
  REQUIRE(callers.find("bar") != callers.end());
}

TEST_CASE("CallGraph: test getCallers by ASTFunction*"
          "[CallGraph]") {
  std::stringstream program;
  program << R"(
      foo(x) {
        return x;
      }
      bar() {
        return foo(7);
      }
    )";

  /* Call graph should be:
   *   bar->foo	direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  ASTFunction *callee = callGraph.get()->getASTFun("foo");
  ASTFunction *caller = callGraph.get()->getASTFun("bar");

  std::set<ASTFunction *> callers = callGraph.get()->getCallers(callee);
  REQUIRE(callers.size() == 1);

  // bar is one of foo's callers
  REQUIRE(callers.find(caller) != callers.end());
}

TEST_CASE("CallGraph: test getEdges"
          "[CallGraph]") {
  std::stringstream program;
  program << R"(
      foo1(x) {
        return x;
      }
      foo2(x) {
              return foo1(x);
      }
      bar() {
        return foo2(7);
      }
    )";

  /* Call graph should be:
   *   bar->foo2	direct call
   *   foo2->foo1	direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto symTable = SymbolTable::build(ast.get());
  auto callGraph = CallGraph::build(ast.get(), symTable.get());

  ASTFunction *foo2 = callGraph.get()->getASTFun("foo2");
  ASTFunction *foo1 = callGraph.get()->getASTFun("foo1");
  ASTFunction *bar = callGraph.get()->getASTFun("bar");

  std::vector<std::pair<ASTFunction *, ASTFunction *>> edges =
      callGraph.get()->getEdges();
  REQUIRE(edges.size() == 2);

  // check for the two edges
  REQUIRE(std::find(edges.begin(), edges.end(), std::make_pair(foo2, foo1)) !=
          edges.end());
  REQUIRE(std::find(edges.begin(), edges.end(), std::make_pair(bar, foo2)) !=
          edges.end());
}

TEST_CASE("CallGraph: test SemanticAnalysis"
          "[CallGraph]") {
  std::stringstream program;
  program << R"(
      foo1(x) {
        return x;
      }
      foo2(x) {
              return foo1(x);
      }
      bar() {
        return foo2(7);
      }
    )";

  auto ast = ASTHelper::build_ast(program);
  auto analysisResults = SemanticAnalysis::analyze(ast.get(), false);
  auto callGraph = analysisResults.get()->getCallGraph();
  REQUIRE(callGraph->getVertices().size() == 3); // size should be 2
}

TEST_CASE("CallGraph: test print method"
          "[CallGraph]") {
  std::stringstream program;
  program << R"(
      foo1(x) {
        return x;
      }
      foo2(x) {
              return foo1(x);
      }
      bar() {
        return foo2(7);
      }
    )";

  /* Call graph should be:
   *   bar->foo2	direct call
   *   foo2->foo1	direct call
   */

  auto ast = ASTHelper::build_ast(program);
  auto analysisResults = SemanticAnalysis::analyze(ast.get(), false);
  auto callGraph = analysisResults.get()->getCallGraph();

  std::stringstream outputStream;
  callGraph->print(outputStream);
  std::string output = outputStream.str();

  // The following are the elements expected in the dot file format

  std::size_t found = output.find("digraph CFG{");
  REQUIRE(found != std::string::npos);

  found = output.find("a0 [label=\"foo1\"];");
  REQUIRE(found != std::string::npos);
  found = output.find("a1 [label=\"foo2\"];");
  REQUIRE(found != std::string::npos);
  found = output.find("a2 [label=\"bar\"];");
  REQUIRE(found != std::string::npos);
  found = output.find("a2 -> a1;");
  REQUIRE(found != std::string::npos);
  found = output.find("a1 -> a0;");
  REQUIRE(found != std::string::npos);
}
