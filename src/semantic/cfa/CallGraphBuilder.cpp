#include "CallGraphBuilder.h"
#include "loguru.hpp"


CallGraphBuilder::CallGraphBuilder(CFAnalyzer p) : cfa(p){}

CallGraphBuilder CallGraphBuilder::build(ASTProgram* ast, CFAnalyzer cfa) {
  CallGraphBuilder cgb(cfa);
  ast->accept(&cgb);
  return cgb;
}

bool CallGraphBuilder::visit(ASTFunction *element) {
  cfun = element;
  return true;
}

bool CallGraphBuilder::visit(ASTFunAppExpr *element) {
  for(ASTFunction* f : cfa.getPossibleFunctionsForExpr(element -> getFunction(), cfun)){
    LOG_S(1) << "Generating call graph edge: " << *cfun << " \u2192 " << *f << " , for call " << *element;
    graph[cfun].insert(f);
    fromFunNameToASTFun[cfun->getName()]= cfun;
    fromFunNameToASTFun[f->getName()]=f;
  }
  return true;
}  // LCOV_EXCL_LINE

std::map<ASTFunction*, std::set<ASTFunction*>> CallGraphBuilder::getCallGraph(){
  return graph;
 }

std::map<std::string, ASTFunction*> CallGraphBuilder::getFunMap() {
  return fromFunNameToASTFun;
}
