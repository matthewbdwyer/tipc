#pragma once

#include <map>
#include <sstream>
#include <iostream>
#include "TIPtree.h"
#include "TIPtypes.h"

class UnionFindSolver {
    //mapping from node to root 
    std::map<std::string, std::string> node2root;
    //mapping from node to type
    std::map<std::string, TIPtype*> node2type;
    std::string findRoot(TIPtree::Node* node);
public: 
    void addNode(TIPtree::Node* node);
    void unifyNodes(TIPtree::Node* nodex, TIPtree::Node* nodey);
    void setType(TIPtree::Node* node, TIPtype* type);
    TIPtype* getType(TIPtree::Node* node);
    bool sameType(TIPtype* typex, TIPtype* typey);
};