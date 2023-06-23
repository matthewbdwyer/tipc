#include "PolyTypeConstraintCollectVisitor.h"
#include "ASTHelper.h"
#include "SymbolTable.h"
#include "CallGraph.h"
#include "TypeConstraintCollectVisitor.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>
#include <set>

TEST_CASE("PolyTypeConstraintVisitor: polymorphic identity function", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(ident(p) {
 return p;
}

main() {
  var x, y;
  x = ident(42);
  y = ident(&x);
  return *x;
})";

    std::vector<std::string> expected{
            "⟦&x@8:12⟧ = ⭡⟦x@6:6⟧",
            "⟦(*x)@9:9⟧ = int",
            "⟦42@7:12⟧ = int",
            "⟦ident@1:0⟧ = (⟦&x@8:12⟧) -> ⟦ident(&x)@8:6⟧", // Instantiation of generic type for first call
            "⟦ident@1:0⟧ = (⟦42@7:12⟧) -> ⟦ident(42)@7:6⟧", // Instantiation of generic type for second call
            "⟦main@5:0⟧ = () -> ⟦(*x)@9:9⟧",
            "⟦x@6:6⟧ = ⟦ident(42)@7:6⟧",
            "⟦x@6:6⟧ = ⭡⟦(*x)@9:9⟧",
            "⟦y@6:9⟧ = ⟦ident(&x)@8:6⟧"
    };

    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());
    auto unifier = std::make_shared<Unifier>();
    auto cg = CallGraph::build(ast.get(), symbols.get());

    // First generate the monomorphic constraints for ident, since we need them in a unifier
    auto identAST = cg->getASTFun("ident");
    TypeConstraintCollectVisitor identVisitor(symbols.get());
    identAST->accept(&identVisitor);
    auto identCollected = identVisitor.getCollectedConstraints();
    unifier->add(identCollected);
    unifier->solve();

    // Now generate the polymorphic constraints for main
    auto mainAST = cg->getASTFun("main");
    PolyTypeConstraintCollectVisitor mainVisitor(symbols.get(), cg.get(), unifier);
    mainAST->accept(&mainVisitor);
    auto collected = mainVisitor.getCollectedConstraints();

    // Copy the vectors to sets to allow for a single equality test
    std::set<std::string> expectedSet;
    copy(expected.begin(), expected.end(),
         inserter(expectedSet, expectedSet.end()));

    std::set<std::string> collectedSet;
    for (int i = 0; i < collected.size(); i++) {
        std::stringstream stream;
        stream << collected.at(i);
        collectedSet.insert(stream.str());
    }

    REQUIRE(expectedSet == collectedSet);

}

