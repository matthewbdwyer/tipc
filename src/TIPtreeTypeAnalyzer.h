#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "TIPtreeDepthFirstVisitor.h"
#include "TIPtree.h"
#include "TIPtreeTypes.h"

using namespace std;
using namespace TIPtree;

class TIPtreeTypeAnalyzer : public TIPtreeDepthFirstVisitor {
  Program* pm;
  unordered_map<shared_ptr<TIPtreeTypes::Type>, shared_ptr<TIPtreeTypes::Type>> unification;

  unordered_map<Node*, shared_ptr<TIPtreeTypes::Var>> nodeTypeMap;

  // We dont have declarition analysis, so use following to keep track of the mapping between variable name and declarition of current scope
  unordered_map<string, shared_ptr<TIPtreeTypes::IdVar>> globalDeclMap;
  unordered_map<string, unordered_map<
    string, shared_ptr<TIPtreeTypes::IdVar>
  >> scopedDeclMap;
  string activeScope;

  shared_ptr<TIPtreeTypes::Var> ast2Type(Node *node);
  shared_ptr<TIPtreeTypes::IdVar> name2Type(string name);
  void unify(shared_ptr<TIPtreeTypes::Type> type1, shared_ptr<TIPtreeTypes::Type> type2);
  shared_ptr<TIPtreeTypes::Type> findTypeRep(shared_ptr<TIPtreeTypes::Type> t);
  shared_ptr<TIPtreeTypes::Type> closeRecType(shared_ptr<TIPtreeTypes::Type>);

public:
  TIPtreeTypeAnalyzer(Program* pm);
  void analyze();

  void visit(Program *pm) override;
  void visit(Function *fn) override;

  void visit(Node *node) override;
  void visit(Expr *expr) override;
  void visit(Stmt *stmt) override;
};

