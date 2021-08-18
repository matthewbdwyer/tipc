#include "CallGraph.h"
#include "ASTHelper.h"
#include "SymbolTable.h"
#include "SemanticAnalysis.h"

#include <catch2/catch_test_macros.hpp>

#include <set>

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

    // callGraph.get()->print(std::cout);

     REQUIRE(callGraph.get()->getTotalVertices() == 4); // 2 subroutines
     REQUIRE(callGraph.get()->getTotalEdges() == 6);

     //check overapproximations, e.g., edge from h->g and h->f
     REQUIRE(true == callGraph.get()->existEdge("h", "g"));
     REQUIRE(true == callGraph.get()->existEdge("h", "f"));  //check some false properties
     REQUIRE(false == callGraph.get()->existEdge("f", "f"));  //check some false properties

}

TEST_CASE("CallGraph: test getCallee by function name" "[CallGraph]") {
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

     //check the size of callees, also can be checked whether a function is being called by another function
     REQUIRE(callGraph.get()->getCallees("main").size()==3);
     REQUIRE(callGraph.get()->getCallees("h").size()==2);
     REQUIRE(callGraph.get()->getCallees("g").size()==1);
     REQUIRE(callGraph.get()->getCallees("f").size()==0);

}



TEST_CASE("CallGraph: test getCallee by ASTFunction*" "[CallGraph]") {
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

     ASTFunction* caller = callGraph.get()->getASTFun("main");
     ASTFunction* callee = callGraph.get()->getASTFun("h");

     std::set<ASTFunction*> callees = callGraph.get()->getCallees(caller);
     REQUIRE(callees.size() == 3); // as main called h,g,f, size should be 3
     REQUIRE(true == (callees.find(callee)!= callees.end())); // h should be in the callee set
}

TEST_CASE("CallGraph: test getCallers" "[CallGraph]") {
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

     //ASTFunction* callee = (ASTFunction*) symTable.get()->getFunction("foo");
     //ASTFunction* caller = (ASTFunction*) symTable.get()->getFunction("bar");

     std::set<std::string> callers = callGraph.get()->getCallers("foo");
     REQUIRE(callers.size() == 1); // as bar called foo, size should be 1
     REQUIRE(true == (callers.find("bar")!= callers.end())); // bar should be in the set
}


TEST_CASE("CallGraph: test getCallers by ASTFunction*" "[CallGraph]") {
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

     ASTFunction* callee = callGraph.get()->getASTFun("foo");
     ASTFunction* caller = callGraph.get()->getASTFun("bar");

     std::set<ASTFunction*> callers = callGraph.get()->getCallers(callee);
     REQUIRE(callers.size() == 1); // as bar called foo, size should be 1
     REQUIRE(true == (callers.find(caller)!= callers.end())); // bar should be in the set
}

TEST_CASE("CallGraph: test getEdges" "[CallGraph]") {
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
     auto symTable = SymbolTable::build(ast.get());
     auto callGraph = CallGraph::build(ast.get(), symTable.get());

     ASTFunction* foo2 = callGraph.get()->getASTFun("foo2");
     ASTFunction* foo1 = callGraph.get()->getASTFun("foo1");
     ASTFunction* bar = callGraph.get()->getASTFun("bar");

     std::vector<std::pair<ASTFunction*, ASTFunction*>> edges = callGraph.get()->getEdges();
     REQUIRE(edges.size() == 2); //size should be 2

     //check if two edges in the graph
     REQUIRE(true ==(std::find(edges.begin(), edges.end(), std::make_pair(foo2, foo1))!=edges.end()));
     REQUIRE(true ==(std::find(edges.begin(), edges.end(), std::make_pair(bar, foo2))!=edges.end()));
}

TEST_CASE("CallGraph: test SemanticAnalysis" "[CallGraph]") {
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
    auto analysisResults = SemanticAnalysis::analyze(ast.get());
    auto callGraph = analysisResults.get()->getCallGraph();
    REQUIRE(callGraph->getVertices().size() == 3); //size should be 2
}

TEST_CASE("CallGraph: test print method" "[CallGraph]") {
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
    auto analysisResults = SemanticAnalysis::analyze(ast.get());
    auto callGraph = analysisResults.get()->getCallGraph();

    std::stringstream outputStream;
    callGraph->print(outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("digraph CFG{");
    REQUIRE(found!=std::string::npos);

    found = output.find("a0 [label=\"foo1\"];");
    REQUIRE(found!=std::string::npos);
    found = output.find("a1 [label=\"foo2\"];");
    REQUIRE(found!=std::string::npos);
    found = output.find("a2 [label=\"bar\"];");
    REQUIRE(found!=std::string::npos);
    found = output.find("a2 -> a1;");
    REQUIRE(found!=std::string::npos);
    found = output.find("a1 -> a0;");
    REQUIRE(found!=std::string::npos);
}
