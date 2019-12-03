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
  vector<Type> getArgs() {
    return vector<Type>();
  };
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
};

}
