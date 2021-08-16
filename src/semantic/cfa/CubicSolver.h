#include <set>
#include <vector>
#include <map>
#include <utility>
#include "ASTNode.h"
#include "ASTFunction.h"

class CubicSolver;

class CubicSolverNode{
private:
    friend CubicSolver;
    CubicSolverNode(int count);
    std::set<CubicSolverNode*> supsets;
    std::set<CubicSolverNode*> subsets;
    bool* bitvector;
    int size;
    std::vector<std::pair<ASTNode*, ASTNode*>>* conditionalConstraints;
};

class CubicSolver{
public:
    CubicSolver(std::vector<ASTFunction*> functions);
    void addElementofConstraint(ASTFunction*, ASTNode*);
    void addConditionalConstraint(ASTFunction*, ASTNode* in, ASTNode* from, ASTNode* to);
    void addSubseteqConstraint(ASTNode*, ASTNode*);
    std::vector<ASTFunction*> getPossibleFunctionsForExpr(ASTNode*);
private:
    void activateConditionalConstraint(ASTNode*, ASTNode*);
    void addEmptyVariableIfNecessary(ASTNode*);
    CubicSolverNode* killCyclesAt(CubicSolverNode*);
    std::vector<CubicSolverNode*> findPath(CubicSolverNode* source, CubicSolverNode* target);
    CubicSolverNode* mergePath(std::vector<CubicSolverNode*>&);
    CubicSolverNode* mergeNodes(CubicSolverNode*, CubicSolverNode*);
    void propegateNodeChanges(CubicSolverNode*);
    std::map<ASTFunction*, int> fmapping;
    std::map<ASTNode*, CubicSolverNode*> dagmapping;
};
