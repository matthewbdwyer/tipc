#include "TIPtreeTypeAnalyzer.h"

TIPtreeTypeAnalyzer::TIPtreeTypeAnalyzer(Program* pm) : pm{pm} {}

void TIPtreeTypeAnalyzer::analyze() {
  visit(pm);
}

void TIPtreeTypeAnalyzer::visit(Program *pm) {
  for (auto &fn : pm->getFunctions()) {
    string name = fn->getName();
    // track function variable name in declarision map
    funDeclMap[name] = make_shared<TIPtreeTypes::IdVar>(name);
  }

  visitChildren(pm);
}

void TIPtreeTypeAnalyzer::visit(Function *fn) {
  // renew declarision map for current function scope
  declMap.clear();
  // add arguments to declarision map
  for (auto formal : fn->getFormals()) {
    declMap[formal] = make_shared<TIPtreeTypes::IdVar>(formal);
  }

  // create types for formals & return
  vector<shared_ptr<TIPtreeTypes::Type>> formalTypes;
  for (auto formal : fn->getFormals()) {
    formalTypes.push_back(name2Type(formal));
  }
  auto returnType = ast2Type(fn->getReturn()->getArg().get());

  // ensure formal constriants of main being added after declarision map
  if (fn->getName() == "main") {
    for (auto formalType : formalTypes) {
      unify(formalType, make_shared<TIPtreeTypes::Int>());
    }
    unify(returnType, make_shared<TIPtreeTypes::Int>());
  }

  // general function declarition constraint
  auto funType = make_shared<TIPtreeTypes::Fun>(formalTypes, returnType);

  unify(name2Type(fn->getName()), funType);

  visitChildren(fn);
}

void TIPtreeTypeAnalyzer::visit(Node *node) {
  if (auto *expr = dynamic_cast<Expr*>(node)) {
    cout << "visit(expr)\n";
    visit(expr);
  } else if (auto *stmt = dynamic_cast<Stmt*>(node)) {
    cout << "visit(stmt)\n";
    visit(stmt);
  } else {
    visitChildren(node);
  }
}

void TIPtreeTypeAnalyzer::visit(Expr *expr) {
  if (auto *ne = dynamic_cast<NumberExpr*>(expr)) {
    unify(ast2Type(ne), make_shared<TIPtreeTypes::Int>());
    visitChildren(ne);
  } else if (auto *be = dynamic_cast<BinaryExpr*>(expr)) {
    if (be->getOp() == "==") {
      unify(ast2Type(be->getLhs().get()), ast2Type(be->getRhs().get()));
    } else {
      unify(ast2Type(be->getLhs().get()), make_shared<TIPtreeTypes::Int>());
      unify(ast2Type(be->getRhs().get()), make_shared<TIPtreeTypes::Int>());
    }
    unify(ast2Type(be), make_shared<TIPtreeTypes::Int>());
    visitChildren(be);
  } else if (auto *fe = dynamic_cast<FunAppExpr*>(expr)) {
    vector<shared_ptr<TIPtreeTypes::Type>> actualTypes;
    for (auto &actual : fe->getActuals()) {
      actualTypes.push_back(ast2Type(actual.get()));
    }
    auto returnType = ast2Type(fe);
    auto funType = make_shared<TIPtreeTypes::Fun>(actualTypes, returnType);

    unify(ast2Type(fe->getFun().get()), funType);
    visitChildren(fe);
  } else if (auto *ie = dynamic_cast<InputExpr*>(expr)) {
    unify(ast2Type(ie), make_shared<TIPtreeTypes::Int>());
    visitChildren(ie);
  } else if (auto *re = dynamic_cast<RefExpr*>(expr)) {
    unify(ast2Type(re), make_shared<TIPtreeTypes::Ref>(name2Type(re->getName())));
    visitChildren(re);
  } else if (auto *de = dynamic_cast<DeRefExpr*>(expr)) {
    unify(ast2Type(de->getArg().get()), make_shared<TIPtreeTypes::Ref>(ast2Type(de)));
    visitChildren(de);
  } else if (auto *ae = dynamic_cast<AllocExpr*>(expr)) {
    unify(ast2Type(ae), make_shared<TIPtreeTypes::Ref>(ast2Type(ae->getArg().get())));
    visitChildren(ae);
  } else if (auto *nle = dynamic_cast<NullExpr*>(expr)) {
    unify(ast2Type(nle), make_shared<TIPtreeTypes::Ref>(make_shared<TIPtreeTypes::Alpha>(ast2Type(nle))));
    visitChildren(nle);
  } else {
    visitChildren(expr);
  }
}

void TIPtreeTypeAnalyzer::visit(Stmt *stmt) {
  if (auto *ds = dynamic_cast<DeclStmt*>(stmt)) {
    // add decl vars to declarision map
    for (auto varName : ds->getVars()) {
      declMap[varName] = make_shared<TIPtreeTypes::IdVar>(varName);
    }
    visitChildren(ds);
  } else if (auto *as = dynamic_cast<AssignStmt*>(stmt)) {
    unify(ast2Type(as->getLhs().get()), ast2Type(as->getRhs().get()));
    visitChildren(as);
  } else if (auto *ws = dynamic_cast<WhileStmt*>(stmt)) {
    unify(ast2Type(ws->getCond().get()), make_shared<TIPtreeTypes::Int>());
    visitChildren(ws);
  } else if (auto *is = dynamic_cast<IfStmt*>(stmt)) {
    unify(ast2Type(is->getCond().get()), make_shared<TIPtreeTypes::Int>());
    visitChildren(is);
  } else if (auto *os = dynamic_cast<OutputStmt*>(stmt)) {
    unify(ast2Type(os->getArg().get()), make_shared<TIPtreeTypes::Int>());
    visitChildren(os);
  } else {
    visitChildren(stmt);
  }
}

shared_ptr<TIPtreeTypes::Var> TIPtreeTypeAnalyzer::ast2Type(Node *node) {
  shared_ptr<TIPtreeTypes::Var> type;

  // use cached term if the ast has a cached type term
  auto nodeSearch = nodeTypeMap.find(node);
  if (nodeSearch != nodeTypeMap.end()) {
    type = nodeSearch->second;
  } else {
    // if ast is a VariableExpr, use cached IdVar
    if (auto *ve = dynamic_cast<VariableExpr*>(node)) {
      type = name2Type(ve->getName());
    } else {
      type = make_shared<TIPtreeTypes::NodeVar>(node);
      nodeTypeMap[node] = type;
    }
  }

  return type;
}

// find predefined type term of a variable name in declarition type map
shared_ptr<TIPtreeTypes::IdVar> TIPtreeTypeAnalyzer::name2Type(string name) {
  shared_ptr<TIPtreeTypes::IdVar> type;

  // if the name has a declarition
  auto declSearch = declMap.find(name);
  if (declSearch != declMap.end()){
    type = declSearch->second;
  } else {
    // if the name has a function declarition
    auto funDeclSearch = funDeclMap.find(name);
    if (funDeclSearch != funDeclMap.end()){
      type = funDeclSearch->second;
    } else {
      std::runtime_error("undeclared variable :" + name);
    }
  }

  return type;
}

void TIPtreeTypeAnalyzer::unify(shared_ptr<TIPtreeTypes::Type> type1, shared_ptr<TIPtreeTypes::Type> type2) {

}
