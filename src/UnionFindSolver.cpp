#include "UnionFindSolver.h"

int UnionFindSolver::findRoot(int node_id) 
{
    addNode(node_id);
    while (node2root[node_id] != node_id) {
        node_id = node2root[node_id];
    }
    return node_id;
}

void UnionFindSolver::addNode(int node_id)
{
    if (node2root.find(node_id) == node2root.end()) {
        node2root[node_id] = node_id;
    }
    if (node2type.find(node_id) == node2type.end()) {
        node2type[node_id] = new TIPalpha();
    }
}

void UnionFindSolver::unifyNodes(int nodex_id, int nodey_id)
{
    addNode(nodex_id);
    addNode(nodey_id);
    int rootx_id = findRoot(nodex_id);
    int rooty_id = findRoot(nodey_id);
    if (rootx_id == rooty_id) {
        return;
    }
    //unify types
    TIPtype* unified_type = unifyTypes(node2type[rootx_id], node2type[rooty_id]);
    node2root[rootx_id] = rooty_id;
    node2type[rooty_id] = unified_type;
}

void UnionFindSolver::setType(int node_id, TIPtype* type) 
{
    addNode(node_id);
    int root_id = findRoot(node_id);
    // unify types
    TIPtype* unified_type = unifyTypes(node2type[root_id], type);
    node2type[root_id] = unified_type;
}

TIPtype* UnionFindSolver::getType(int node_id) 
{
    addNode(node_id);
    int root_id = findRoot(node_id);
    return node2type[root_id];
}

bool UnionFindSolver::existNode(int node_id)
{
    return node2root.find(node_id) != node2root.end();
}

TIPtype* UnionFindSolver::unifyTypes(TIPtype* typex, TIPtype* typey)
{
    if (typex->print() == typey->print()) {
        //same type
        return typex;
    }
    if (typex->print() == TIPalpha::ALPHA) {
        //x is untyped
        return typey;
    }
    if (typey->print() == TIPalpha::ALPHA) {
        //y is untyped
        return typex;
    }
    if (!typex->composite || !typey->composite) {
        throw TIPTypeError("Cannot unify types " + typex->print() +" and " + typey->print());
    }
    //unify functions
    TIPfun* funx = dynamic_cast<TIPfun*>(typex);
    TIPfun* funy = dynamic_cast<TIPfun*>(typey);
    if (funx->param_types.size() != funy->param_types.size()) {
        throw TIPTypeError("Cannot unify types " + typex->print() +" and " + typey->print());
    }
    std::vector<TIPtype*> merged_param_types;
    for (int i=0; i<funx->param_types.size(); i++) {
        merged_param_types.push_back(unifyTypes(funx->param_types[i], funy->param_types[i]));
    }
    TIPtype* merged_ret = unifyTypes(funx->ret, funy->ret);
    return new TIPfun(merged_param_types, merged_ret);
}