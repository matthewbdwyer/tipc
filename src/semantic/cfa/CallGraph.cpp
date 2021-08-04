#include "CallGraph.h"

int CallGraph::getTotalVertices()
{
    return vertices.size();
}

int CallGraph::getTotalEdges()
{
    return total_edges;
}

std::vector<ASTFunction*> CallGraph::getVertices()
{
    return vertices;
}

std::vector<std::pair<ASTFunction*, ASTFunction*>> CallGraph::getEdges()
{
    return edges;
}

//std::set<ASTFunction*> CallGraph::getCallees(ASTFunction* f)
//{
   //return null; //callGraph.get(f);
//}

//std::set<ASTFunction*> CallGraph::getCallers(ASTFunction* f)
//{

//}

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

void CallGraph::setTotalEdges()
{
    int total = 0;
    for (auto pair : callGraph) {
        for (auto dest : pair.second)
            total++;
    }
    total_edges = total;
}
