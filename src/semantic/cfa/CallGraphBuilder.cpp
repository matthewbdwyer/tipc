//
// contributor: Spencer Martin
// modified by SBH

#include "CGB.h"
#include "loguru.hpp"


std::map<ASTFunction*, std::set<ASTFunction*>> CallGraphBuilder::build(ASTProgram* ast, CFAnalyzer cfa){
    CallGraphBuilder cgb(cfa);
    ast -> accept(&cgb);
    return cgb.graph;
}


CallGraphBuilder::CallGraphBuilder(CFAnalyzer p) : cfa(p){}

bool CallGraphBuilder::visit(ASTFunction *element) {
    cfun = element;
    return true;
}

bool CallGraphBuilder::visit(ASTFunAppExpr *element) {
    for(ASTFunction* f : cfa.getPossibleFunctionsForExpr(element -> getFunction(), cfun)){
        graph[cfun].insert(f);
    }
    return true;
}

bool CallGraphBuilder::visit(ASTVariableExpr *element) {
    for(ASTFunction* f : cfa.getPossibleFunctionsForExpr(element, cfun)){
        graph[cfun].insert(f);
    }
    return true;
}

bool CallGraphBuilder::visit(ASTReturnStmt *element) {
    for(ASTFunction* f : cfa.getPossibleFunctionsForExpr(element, cfun)){
        graph[cfun].insert(f);
    }
    return true;
}



