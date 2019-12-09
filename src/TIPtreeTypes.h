#pragma once

#include <string>
#include <vector>

namespace TIPtreeTypes {

class Type {
public:
  virtual std::string print() = 0;
};

// Var type
class Var : public Type {};

class NodeVar : public Var {
  std::string nodeStr;
public:
  NodeVar(std::string nodeStr) : nodeStr{nodeStr} {}

  std::string print() {
    return "[[" + nodeStr + "]]";
  }
};

class IdVar : public Var {
  std::string name;
public:
  IdVar(std::string name) : name(name) {}

  std::string print() {
    return "[[" + name + "]]";
  }
};

class Alpha : public Var {
  std::shared_ptr<Var> x;
public:
  Alpha(std::shared_ptr<Var> x) : x(move(x)) {}

  std::string print() {
    return "\u03B1<" + x->print() + ">";
  };
};

// Con type
class Con : public Type {
public:
  virtual std::vector<std::shared_ptr<Type>> getArgs() {
    return std::vector<std::shared_ptr<Type>>();
  };

  virtual void subst(std::vector<std::shared_ptr<Type>> newArgs) {};
};

class Int : public Con {
public:
  std::string print() {
    return "int";
  }
};

class Ref : public Con {
  std::shared_ptr<Type> targetType;
public:
  Ref(std::shared_ptr<Type> targetType) : targetType(move(targetType)) {}

  std::string print() {
    return "&" + targetType->print();
  };

  std::vector<std::shared_ptr<Type>> getArgs() {
    return std::vector<std::shared_ptr<Type>>({targetType});
  };

  void subst(std::vector<std::shared_ptr<Type>> newArgs) {
    targetType = newArgs[0];
  };
};

class Fun : public Con {
  std::vector<std::shared_ptr<Type>> formalTypes;
  std::shared_ptr<Type> returnType;
public:
  Fun(std::vector<std::shared_ptr<Type>> formalTypes, std::shared_ptr<Type> returnType) : formalTypes(move(formalTypes)), returnType(move(returnType)) {}

  std::string print() {
    // comma separated parameter type list
    std::string ft = "";
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

  std::vector<std::shared_ptr<Type>> getArgs() {
    std::vector<std::shared_ptr<Type>> args(formalTypes);
    args.push_back(returnType);
    return args;
  };

  void subst(std::vector<std::shared_ptr<Type>> newArgs) {
    returnType = newArgs.back();
    newArgs.pop_back();
    formalTypes = newArgs;
  };
};

}
