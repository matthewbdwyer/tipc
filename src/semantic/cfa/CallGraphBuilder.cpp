#include "CallGraphBuilder.h"
#include "loguru.hpp"

CallGraphBuilder CallGraphBuilder::build(ASTProgram *ast, CFAnalyzer cfa) {
  CallGraphBuilder cgb(cfa);
  ast->accept(&cgb);
  return cgb;
}

CallGraphBuilder::CallGraphBuilder(CFAnalyzer p) : cfa(p) {}

bool CallGraphBuilder::visit(ASTFunction *element) {
  cfun = element;
  return true;
}

bool CallGraphBuilder::visit(ASTFunAppExpr *element) {
  std::set<ASTFunction *> called;
  for (ASTFunction *f :
       cfa.getPossibleFunctionsForExpr(element->getFunction(), cfun)) {
    LOG_S(1) << "Call graph builder: adding " << *cfun << " -> " << *f
             << " based on call " << *element;
    called.emplace(f);
    graph[cfun].insert(f);
    fromFunNameToASTFun[cfun->getName()] = cfun;
    fromFunNameToASTFun[f->getName()] = f;
  }
  mayCall.insert(
      std::pair<ASTFunAppExpr *, std::set<ASTFunction *>>(element, called));
  return true;
} // LCOV_EXCL_LINE

std::map<ASTFunction *, std::set<ASTFunction *>>
CallGraphBuilder::getCallGraph() {
  return graph;
}

std::map<ASTFunAppExpr *, std::set<ASTFunction *>>
CallGraphBuilder::getMayCall() {
  return mayCall;
}

std::map<std::string, ASTFunction *> CallGraphBuilder::getFunMap() {

  return fromFunNameToASTFun;
}
