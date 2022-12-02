#include "CallGraphBuilder.h"
#include "loguru.hpp"


CallGraphBuilder CallGraphBuilder::build(ASTProgram* ast, CFAnalyzer cfa){
    CallGraphBuilder cgb(cfa);
    ast -> accept(&cgb);
    return cgb;//.graph;
}


CallGraphBuilder::CallGraphBuilder(CFAnalyzer p) : cfa(p){}

bool CallGraphBuilder::visit(ASTFunction *element) {
    cfun = element;
    return true;
}

bool CallGraphBuilder::visit(ASTFunAppExpr *element) {
    for(ASTFunction* f : cfa.getPossibleFunctionsForExpr(element -> getFunction(), cfun)){
        graph[cfun].insert(f);
        fromFunNameToASTFun[cfun->getName()]= cfun;
        fromFunNameToASTFun[f->getName()]=f;

    }
    return true;
}  // LCOV_EXCL_LINE

bool CallGraphBuilder::visit(ASTVariableExpr *element) {
    for(ASTFunction* f : cfa.getPossibleFunctionsForExpr(element, cfun)){
        graph[cfun].insert(f);
           fromFunNameToASTFun[cfun->getName()]= cfun;
               fromFunNameToASTFun[f->getName()]=f;
    }
    return true;
}  // LCOV_EXCL_LINE

std::map<ASTFunction*, std::set<ASTFunction*>> CallGraphBuilder::getCallGraph(){

  return graph;
}

std::map<std::string, ASTFunction*> CallGraphBuilder::getFunMap(){

  return fromFunNameToASTFun;

}



