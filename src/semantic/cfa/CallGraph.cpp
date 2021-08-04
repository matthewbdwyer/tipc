#include "CallGraph.h"
#include <iostream>

std::unique_ptr<CallGraph> CallGraph::build(ASTProgram* ast, SymbolTable* st){

    auto cfa = CFAnalyzer::analyze(ast,st);
    auto callGraph = CallGraphBuilder::build(ast,cfa);
    return std::make_unique<CallGraph>(callGraph, ast -> getFunctions());
}

int CallGraph::getTotalVertices()
{
    return total_vertices;
}

int CallGraph::getTotalEdges()
{
    int total_edges = 0;
    for (auto pair : callGraph) {
        for (auto dest : pair.second)
            total_edges++;
    }
    return total_edges;
}

std::vector<ASTFunction*> CallGraph::getVertices()
{
    return vertices;
}

std::vector<std::pair<ASTFunction*, ASTFunction*> > CallGraph::getEdges()
{
    for (auto pair : callGraph) {
        for (auto dest : pair.second) {
            std::pair edge = std::make_pair(pair.first, dest);
            edges.push_back(edge);
        }
    }
    return edges;
}

std::set<ASTFunction*> CallGraph::getCallees(ASTFunction* f)
{
   return callGraph.find(f)->second;
}

std::set<ASTFunction*> CallGraph::getCallers(ASTFunction* f)
{
   return callGraph.find(f)->second;
}

void CallGraph::print(std::ostream& str)
{
    str << "digraph CFG{\n";
    std::map<ASTFunction*, int> inds;
    int i = 0;

    for (auto f : vertices) {
        inds[f] = i++;
        str << "a" << inds[f] << " [label=\"" << f->getName() << "\"];\n";
    }
    for (auto pair : callGraph) {
        for (auto dest : pair.second) {
            str << "a" << inds[pair.first] << " -> a" << inds[dest] << ";\n";
        }
    }
    str << "}\n";
}

bool CallGraph::existEdge(std::string caller, std::string callee)
{
      for(std::pair<ASTFunction*, ASTFunction*> edge : getEdges()){
         if((edge.first->getName().compare(caller) == 0) && (edge.second->getName().compare(callee) == 0))
            return true;
      }
      return false;
}