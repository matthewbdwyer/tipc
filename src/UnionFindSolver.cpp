#include "UnionFindSolver.h"

std::string UnionFindSolver::findRoot(TIPtree::Node* node) 
{
    addNode(node);
    std::string node_str = node->print();
    while (node2root[node_str] != node_str) {
        node_str = node2root[node_str];
    }
    return node_str;
}

void UnionFindSolver::addNode(TIPtree::Node* node)
{
    std::string node_str = node->print();
    if (node2root.find(node_str) == node2root.end()) {
        node2root[node_str] = node_str;
    }
    if (node2type.find(node_str) == node2type.end()) {
        node2type[node_str] = nullptr;
    }
}

void UnionFindSolver::unifyNodes(TIPtree::Node* nodex, TIPtree::Node* nodey)
{
    addNode(nodex);
    addNode(nodey);
    std::string rootx_str = findRoot(nodex);
    std::string rooty_str = findRoot(nodey);
    if (rootx_str == rooty_str) {
        return;
    }
    // rootx or rooty does not have a type yet
    if (node2type[rootx_str] == nullptr) {
        node2type[rootx_str] = node2type[rooty_str];
        node2root[rootx_str] = rooty_str;
        return;
    }
    if (node2type[rooty_str] == nullptr) {
        node2type[rooty_str] = node2type[rootx_str];
        node2root[rooty_str] = rootx_str;
        return;
    }
    // type mismatch
    if (!sameType(node2type[rootx_str], node2type[rooty_str])) {
        // TODO: move information from std::cout to logger
        std::ostringstream oss;
        oss << "type mismatch: " << nodex->print() << "(" << node2type[rootx_str]->toStr() << ") does not match " << nodey->print() << "(" << node2type[rooty_str]->toStr() << ")";
        std::string mismatch_information = oss.str();
        std::cout << mismatch_information << std::endl;
        return;
    } else {
        node2root[rootx_str] = rooty_str;
    }
}

void UnionFindSolver::setType(TIPtree::Node* node, TIPtype* type) 
{
    std::string root_str = findRoot(node);
    // root does not have type yet
    if (node2type[root_str] == nullptr) {
        node2type[root_str] = type;
        return;
    }
    // root type mismatch
    if (!sameType(node2type[root_str], type)) {
        // TODO: move information from std::cout to logger
        std::ostringstream oss;
        oss << "type mismatch: " << root_str << "(" << node2type[root_str]->toStr() << ") does not match type: " << type->toStr();
        std::string mismatch_information = oss.str();
        std::cout << mismatch_information << std::endl;
    }
}

TIPtype* UnionFindSolver::getType(TIPtree::Node* node) 
{
    std::string root_str = findRoot(node);
    return node2type[root_str];
}

bool UnionFindSolver::sameType(TIPtype* typex, TIPtype* typey)
{
    if (typex == typey) {
        return true;
    }
    if (typex == nullptr || typey == nullptr) {
        return false;
    }
    return typex->toStr() == typey->toStr();
}

