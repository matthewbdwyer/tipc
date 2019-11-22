#include <map>
#include <sstream>
#include <iostream>
#include "TIPtree.h"
#include "TIPtypes.h"

class UnionFindSolver {
private:
    //constraint relationship between ast nodes
    std::map<TIPtree::Node*, TIPtree::Node*> parent;
    //mapping from ast node to type
    std::map<TIPtree::Node*, TIPtype*> nodetype;
public:    
    void makeSet(TIPtree::Node* node);
    TIPtree::Node* findParent(TIPtree::Node* node);
    void unifyNodes(TIPtree::Node* nodex, TIPtree::Node* nodey);
    void setType(TIPtree::Node* node, TIPtype* type);
    //std::string typeMismatchInfo(TIPtree::Node* nodex, TIPtree::Node* nodey);
};