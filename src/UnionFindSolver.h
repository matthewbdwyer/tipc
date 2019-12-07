#pragma once

#include <map>
#include <sstream>
#include <iostream>
#include "TIPtree.h"
#include "TIPtypes.h"

class UnionFindSolver {
    //mapping from node to root 
    std::map<int, int> node2root;
    //mapping from node to type
    std::map<int, TIPtype*> node2type;
public: 
    int findRoot(int node_id);
    void addNode(int node_id);
    void unifyNodes(int nodex_id, int nodey_id);
    void setType(int node_id, TIPtype* type);
    TIPtype* getType(int node_id);
    TIPtype* unifyTypes(TIPtype* typex, TIPtype* typey);
    bool existNode(int node_id);
};