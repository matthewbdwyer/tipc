#include "CubicSolver.h"
#include "loguru.hpp"
#include <map>
#include <queue>

CubicSolverNode::CubicSolverNode(int count) {
  bitvector.resize(count);
  conditionalConstraints.resize(count);
  for (int i = 0; i < count; i++) {
    bitvector[i] = false;
  }
  size = count;
}

CubicSolver::CubicSolver(std::vector<ASTFunction *> functions) {
  for (int i = 0; i < functions.size(); i++) {
    fmapping[functions[i]] = i;
  }
}

void CubicSolver::addEmptyVariableIfNecessary(ASTNode *node) {
  if (dagmapping.find(node) != dagmapping.end()) {
    return;
  }
  dagmapping[node] = std::make_shared<CubicSolverNode>(fmapping.size());
}

void CubicSolver::addElementofConstraint(ASTFunction *fn, ASTNode *node) {
  LOG_S(1) << "Generating control flow constraint: " << fn->getName()
           << " \u2208 \u27e6" << *node << "\u27e7";
  addEmptyVariableIfNecessary(node);
  dagmapping[node]->bitvector[fmapping[fn]] = true;
  propagateNodeChanges(dagmapping[node]);
}

void CubicSolver::addConditionalConstraint(ASTFunction *condition, ASTNode *in,
                                           ASTNode *from, ASTNode *to) {
  LOG_S(1) << "Generating control flow constraint: " << condition->getName()
           << " \u2208 \u27e6" << *in << "\u27e7 \u21d2 \u27e6" << *from
           << "\u27e7 \u2286 \u27e6" << *to << "\u27e7";
  addEmptyVariableIfNecessary(in);
  addEmptyVariableIfNecessary(from);
  addEmptyVariableIfNecessary(to);
  dagmapping[in]->conditionalConstraints[fmapping[condition]].push_back(
      std::pair(from, to));
  propagateNodeChanges(dagmapping[in]);
}

void CubicSolver::addSubseteqConstraint(ASTNode *from, ASTNode *to) {
  LOG_S(1) << "Generating control flow constraint: "
           << "\u27e6" << *from << "\u27e7 \u2286 \u27e6" << *to << "\u27e7";
  addEmptyVariableIfNecessary(from);
  addEmptyVariableIfNecessary(to);
  if (dagmapping[from] == dagmapping[to]) {
    return;
  }
  dagmapping[from]->supsets.insert(dagmapping[to]);
  dagmapping[to]->subsets.insert(dagmapping[from]);
  killCyclesAt(dagmapping[from]);
  propagateNodeChanges(dagmapping[from]);
}

void CubicSolver::propagateNodeChanges(std::shared_ptr<CubicSolverNode> node) {
  for (int i = 0; i < node->size; i++) {
    if (node->bitvector[i]) {
      auto constraints = node->conditionalConstraints[i];
      node->conditionalConstraints[i].clear();
      for (auto pair : constraints) {
        activateConditionalConstraint(pair.first, pair.second);
      }
    }
  }
  for (std::shared_ptr<CubicSolverNode> sups : node->supsets) {
    for (int i = 0; i < node->size; i++) {
      sups->bitvector[i] = sups->bitvector[i] || node->bitvector[i];
    }
    assert(sups != node);
    propagateNodeChanges(sups);
  }
}

void CubicSolver::activateConditionalConstraint(ASTNode *from, ASTNode *to) {
  if (dagmapping[from] == dagmapping[to]) {
    return;
  }
  dagmapping[from]->supsets.insert(dagmapping[to]);
  dagmapping[to]->subsets.insert(dagmapping[from]);
  killCyclesAt(dagmapping[from]);
  propagateNodeChanges(dagmapping[from]);
}

std::shared_ptr<CubicSolverNode>
CubicSolver::killCyclesAt(std::shared_ptr<CubicSolverNode> n) {
  bool collapsed;
  do {
    collapsed = false;
    for (std::shared_ptr<CubicSolverNode> t : n->supsets) {
      auto path = findPath(t, n);
      if (path.size() == 0) {
        continue;
      }
      n = mergePath(path);
      collapsed = true;
      break;
    }
  } while (collapsed);
  return n;
} // LCOV_EXCL_LINE

std::shared_ptr<CubicSolverNode>
CubicSolver::mergePath(std::vector<std::shared_ptr<CubicSolverNode>> &path) {
  assert(path.size() != 0);
  while (true) {
    if (path.size() == 1) {
      return path[0];
    }
    std::shared_ptr<CubicSolverNode> a = path.back();
    path.pop_back();
    std::shared_ptr<CubicSolverNode> b = path.back();
    path.pop_back();
    path.push_back(mergeNodes(a, b));
  }
}

std::shared_ptr<CubicSolverNode>
CubicSolver::mergeNodes(std::shared_ptr<CubicSolverNode> n1,
                        std::shared_ptr<CubicSolverNode> n2) {
  for (auto &pair : dagmapping) {
    if (pair.second == n2) {
      pair.second = n1;
    }
  }
  for (int i = 0; i < n1->size; i++) {
    n1->bitvector[i] = n1->bitvector[i] || n2->bitvector[i];
    for (auto a : n2->conditionalConstraints[i]) {
      n1->conditionalConstraints[i].push_back(a);
    }
  }
  for (auto a : n2->supsets) {
    if (n1 == a) {
      continue;
    }
    a->subsets.erase(a->subsets.find(n2));
    a->subsets.insert(n1);
    n1->supsets.insert(a);
  }
  for (auto a : n2->subsets) {
    if (n1 == a) {
      continue;
    }
    a->supsets.erase(a->supsets.find(n2));
    a->supsets.insert(n1);
    n1->subsets.insert(a);
  }
  if (n1->supsets.find(n2) != n1->supsets.end()) {
    n1->supsets.erase(n1->supsets.find(n2));
  }
  if (n1->subsets.find(n2) != n1->subsets.end()) {
    n1->subsets.erase(n1->subsets.find(n2));
  }
  return n1;
}

std::vector<std::shared_ptr<CubicSolverNode>>
CubicSolver::findPath(std::shared_ptr<CubicSolverNode> source,
                      std::shared_ptr<CubicSolverNode> target) {
  std::map<std::shared_ptr<CubicSolverNode>, int> distances;
  distances[source] = 0;
  std::queue<std::shared_ptr<CubicSolverNode>> q;

  auto populateDistances = [&](std::shared_ptr<CubicSolverNode> node) {
    assert(node != NULL);
    for (std::shared_ptr<CubicSolverNode> n : node->supsets) {
      if (distances.find(n) != distances.end()) {
        continue;
      }
      distances[n] = distances[node] + 1;
      q.push(n);
    }
  };

  populateDistances(source);

  while (!q.empty()) {
    std::shared_ptr<CubicSolverNode> curr = q.front();
    populateDistances(curr);
    q.pop();
    if (distances.find(target) != distances.end()) {
      break;
    }
  }

  std::vector<std::shared_ptr<CubicSolverNode>> out;

  if (distances.find(target) == distances.end()) {
    return out;
  } else {
    std::shared_ptr<CubicSolverNode> curr = target;
    out.push_back(target);
    while (curr != source) {
      for (std::shared_ptr<CubicSolverNode> n : curr->subsets) {
        if (distances.find(n) == distances.end()) {
          continue;
        }
        if (distances[n] == distances[curr] - 1) {
          curr = n;
        }
        break;
      }
      out.push_back(curr);
    }
    return out;
  }
}

std::vector<ASTFunction *>
CubicSolver::getPossibleFunctionsForExpr(ASTNode *n) {
  std::vector<ASTFunction *> out;
  if (dagmapping.find(n) == dagmapping.end()) {
    return out;
  }
  for (auto pair : fmapping) {
    if (dagmapping[n]->bitvector[pair.second]) {
      out.push_back(pair.first);
    }
  }
  return out;
}
