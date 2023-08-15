#include "ASTFunction.h"
#include "ASTNode.h"
#include <map>
#include <set>
#include <utility>
#include <vector>

class CubicSolver;

class CubicSolverNode {
public:
  CubicSolverNode(int count);

private:
  friend CubicSolver;
  std::set<std::shared_ptr<CubicSolverNode>> supsets;
  std::set<std::shared_ptr<CubicSolverNode>> subsets;
  std::vector<bool> bitvector;
  int size;
  std::vector<std::vector<std::pair<ASTNode *, ASTNode *>>>
      conditionalConstraints;
};

class CubicSolver {
public:
  CubicSolver(std::vector<ASTFunction *> functions);
  void addElementofConstraint(ASTFunction *fn, ASTNode *node);
  void addConditionalConstraint(ASTFunction *condition, ASTNode *in,
                                ASTNode *from, ASTNode *to);
  void addSubseteqConstraint(ASTNode *from, ASTNode *to);
  std::vector<ASTFunction *> getPossibleFunctionsForExpr(ASTNode *);

private:
  void activateConditionalConstraint(ASTNode *from, ASTNode *to);
  void addEmptyVariableIfNecessary(ASTNode *node);
  std::shared_ptr<CubicSolverNode>
  killCyclesAt(std::shared_ptr<CubicSolverNode> n);
  std::vector<std::shared_ptr<CubicSolverNode>>
  findPath(std::shared_ptr<CubicSolverNode> source,
           std::shared_ptr<CubicSolverNode> target);
  std::shared_ptr<CubicSolverNode>
  mergePath(std::vector<std::shared_ptr<CubicSolverNode>> &path);
  std::shared_ptr<CubicSolverNode>
  mergeNodes(std::shared_ptr<CubicSolverNode> n1,
             std::shared_ptr<CubicSolverNode> n2);
  void propagateNodeChanges(std::shared_ptr<CubicSolverNode> node);
  std::map<ASTFunction *, int> fmapping;
  std::map<ASTNode *, std::shared_ptr<CubicSolverNode>> dagmapping;
};
