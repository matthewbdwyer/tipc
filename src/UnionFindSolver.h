#pragma once

#include <map>
#include <sstream>
#include <iostream>
#include "TIPtree.h"
#include "TIPtypes.h"

class UnionFindSolver {
    std::string funscope = "";
    //mapping from node to root 
    std::map<std::string, std::string> node2root;
    //mapping from node to type
    std::map<std::string, TIPtype*> node2type;
    //mapping from function name to params
    std::map<std::string, std::vector<std::string>> fun2params;
    //mapping from function name to return value
    std::map<std::string, std::string> fun2ret;
    std::string findRoot(std::string node_str);
    TIPfun* mergeFuns(TIPtype* typex, TIPtype* typey);
public: 
    void addNode(std::string node_str);
    void unifyNodes(std::string nodex_str, std::string nodey_str);
    void setType(std::string node_str, TIPtype* type);
    TIPtype* getType(std::string node_str);
    bool sameType(TIPtype* typex, TIPtype* typey);
    bool existNode(std::string node_str);
    void setFunScope(std::string fun_name);
    std::string getFunScope();
};