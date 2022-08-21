#include "CallGraph.h"
#include "loguru.hpp"

std::unique_ptr<CallGraph> CallGraph::build(ASTProgram* ast, SymbolTable* st){
  LOG_S(1) << "Building call graph";
  auto cfa = CFAnalyzer::analyze(ast,st);
  auto cgb = CallGraphBuilder::build(ast,cfa);
  return std::make_unique<CallGraph>(cgb.getCallGraph(), ast -> getFunctions(), cgb.getFunMap());
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
}  // LCOV_EXCL_LINE

std::vector<ASTFunction*> CallGraph::getVertices()
{
    return vertices;
}

std::vector<std::pair<ASTFunction*, ASTFunction*>> CallGraph::getEdges()
{
    for (auto pair : callGraph) {
        for (auto dest : pair.second) {
            std::pair edge = std::make_pair(pair.first, dest);
            edges.push_back(edge);
        }
    }
    return edges;
}  // LCOV_EXCL_LINE

std::set<ASTFunction*> CallGraph::getCallees(ASTFunction* f)
{
   return callGraph.find(f)->second;
}

std::set<ASTFunction*> CallGraph::getCallees(std::string caller)
{
   //iterate map and if function name matches return the list
   std::set<ASTFunction*> callees;
   for (auto fun : callGraph)
      if(fun.first->getName().compare(caller) == 0) callees=callGraph.find(fun.first)->second;
   return callees;
}

std::set<std::string> CallGraph::getCallers(std::string callee)
{
      std::set<std::string> callers;
      for (auto pair : callGraph){
         for (auto callee_ : pair.second) {
               if(callee_->getName().compare(callee) == 0)
                 callers.insert(pair.first->getName());
         }
      }
      return callers;
}

std::set<ASTFunction*> CallGraph::getCallers(ASTFunction* f)
{

      std::set<ASTFunction*> callers;
      for (auto pair : callGraph){
         if(pair.second.find(f)!=pair.second.end()) callers.insert(pair.first);
      }
      return callers;
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

ASTFunction* CallGraph::getASTFun(std::string f_name)
{
      return fromFunNameToASTFuns[f_name];
}
