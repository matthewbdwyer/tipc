#include "UnionFindSolver.h"

void UnionFindSolver::makeSet(TIPtree::Node* node)
{
    if (parent.find(node) == parent.end()) {
        parent[node] = node;
    }
    if (nodetype.find(node) == nodetype.end()) {
        nodetype[node] = nullptr;
    }
}

TIPtree::Node* UnionFindSolver::findParent(TIPtree::Node* node) 
{
    makeSet(node);
    while (parent[node] != node) {
        node = parent[node];
    }
    return node;
}

void UnionFindSolver::unifyNodes(TIPtree::Node* nodex, TIPtree::Node* nodey)
{
    makeSet(nodex);
    makeSet(nodey);
    TIPtree::Node* parentx = findParent(nodex);
    TIPtree::Node* parenty = findParent(nodey);
    if (*nodetype[parentx] != *nodetype[parenty]) {
        //type mismatch
        std::ostringstream oss;
        oss << "type mismatch:" << parentx->print() << " and " << parenty->print();
        throw oss.str();
    }
    //unify parents
    parent[parentx] = parenty;
}

void UnionFindSolver::setType(TIPtree::Node* node, TIPtype* type) 
{
    TIPtree::Node* parent = findParent(node);
    if (nodetype[parent] != nullptr && *nodetype[parent] != *type) {
        //type mismatch
        std::ostringstream oss;
        oss << "type mismatch: " << node->print() << "cannot have type " << type->toStr();
        throw oss.str(); 
    }
    nodetype[parent] = type;
}
