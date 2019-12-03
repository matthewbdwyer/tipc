#include <unordered_map>
#include "TIPtreeDepthFirstVisitor.h"
#include "TIPtreeTypes.h"

using namespace std;
using namespace TIPtree;

class TIPtreeTypeAnalyzer : public TIPtreeDepthFirstVisitor {
  Program* pm;
  unordered_map<shared_ptr<TIPtreeTypes::Type>, shared_ptr<TIPtreeTypes::Type>> unification;

  unordered_map<Node*, shared_ptr<TIPtreeTypes::Var>> nodeTypeMap;

  // We dont have declarition analysis, so use following to keep track of the mapping between variable name and declarition of current scope
  unordered_map<string, shared_ptr<TIPtreeTypes::IdVar>> declMap;
  unordered_map<string, shared_ptr<TIPtreeTypes::IdVar>> funDeclMap;

  void unify(shared_ptr<TIPtreeTypes::Type> type1, shared_ptr<TIPtreeTypes::Type> type2);
  shared_ptr<TIPtreeTypes::Var> ast2Type(Node *node);
  shared_ptr<TIPtreeTypes::IdVar> name2Type(string name);

public:
  TIPtreeTypeAnalyzer(Program* pm);
  void analyze();

  void visit(Program *pm) override;
  void visit(Function *fn) override;

  void visit(Node *node) override;
  void visit(Expr *expr) override;
  void visit(Stmt *stmt) override;
};

