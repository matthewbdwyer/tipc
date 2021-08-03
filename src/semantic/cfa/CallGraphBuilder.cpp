//
// contributor: Spencer Martin
//

#include "CGB.h"
#include "loguru.hpp"

std::map<ASTFunction*, std::set<ASTFunction*>> CallGraphBuilder::build(ASTProgram* p, CFAnalyzer cfa){
    CallGraphBuilder cgb(cfa);
    p -> accept(&cgb);
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

int CallGraphBuilder::printCallGraph(const std::vector<ASTFunction*>& functions, const std::map<ASTFunction*, std::set<ASTFunction*>>& graph, std::ostream& str){
    str << "digraph CFG{\n";
    std::map<ASTFunction*, int> inds;
    int i = 0;
    int total_edges=0;
    for(auto f : functions){
        inds[f] = i++;
        str << "a" << inds[f] << " [label=\"" << f -> getName() << "\"];\n";
    }
    for(auto pair : graph){
        for(auto dest : pair.second){
            str << "a" << inds[pair.first] << " -> a" << inds[dest] << ";\n";
            total_edges++;
        }
    }
    str << "}\n";
    return total_edges;
}
