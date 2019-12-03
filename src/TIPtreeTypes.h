#include "TIPtree.h"

namespace TIPtreeTypes {

class Type {
public:
  virtual string print() = 0;
};

// Var type
class Var : public Type {};

class NodeVar : public Var {
  TIPtree::Node *node;
public:
  NodeVar(TIPtree::Node *node) : node{node} {}

  string print() {
    return "[[" + node->print() + "]]";
  }
};

class IdVar : public Var {
  std::string name;
public:
  IdVar(string name) : name(name) {}

  string print() {
    return "[[" + name + "]]";
  }
};

class Alpha : public Var {
  shared_ptr<Var> x;
public:
  Alpha(shared_ptr<Var> x) : x(move(x)) {}

  string print() {
    return "\u03B1<" + x->print() + ">";
  };
};

// Con type
class Con : public Type {
public:
  virtual vector<shared_ptr<Type>> getArgs() {
    return vector<shared_ptr<Type>>();
  };

  virtual void subst(vector<shared_ptr<Type>> newArgs) {};
};

class Int : public Con {
public:
  string print() {
    return "int";
  }
};

class Ref : public Con {
  shared_ptr<Type> targetType;
public:
  Ref(shared_ptr<Type> targetType) : targetType(move(targetType)) {}

  string print() {
    return "&" + targetType->print();
  };

  vector<shared_ptr<Type>> getArgs() {
    return vector<shared_ptr<Type>>({targetType});
  };

  void subst(vector<shared_ptr<Type>> newArgs) {
    targetType = newArgs[0];
  };
};

class Fun : public Con {
  vector<shared_ptr<Type>> formalTypes;
  shared_ptr<Type> returnType;
public:
  Fun(vector<shared_ptr<Type>> formalTypes, shared_ptr<Type> returnType) : formalTypes(move(formalTypes)), returnType(move(returnType)) {}

  string print() {
    // comma separated parameter type list
    string ft = "";
    bool skip = true;
    for (auto formalType : formalTypes) {
      if (skip) {
        skip = false;
      } else {
        ft += ", ";
      }
      ft += formalType->print();
    }

    return "(" + ft + ") -> " + returnType->print();
  };

  vector<shared_ptr<Type>> getArgs() {
    vector<shared_ptr<Type>> args(formalTypes);
    args.push_back(returnType);
    return args;
  };

  void subst(vector<shared_ptr<Type>> newArgs) {
    returnType = newArgs.back();
    newArgs.pop_back();
    formalTypes = newArgs;
  };
};

}
