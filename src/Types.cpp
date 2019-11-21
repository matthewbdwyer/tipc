#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class TipType {
public:
    virtual std::string tostr() = 0;
};

class TipInt : public TipType {
public:
    std::string tostr() override {
        return "int";
    }
};

class TipRef : public TipType {
    TipType *of;

public:
    TipRef(TipType *of) : of(of) {}

    std::string tostr() override {
        std::ostringstream oss;
        oss << "&" << of->tostr();
        return oss.str();
    }
};

class TipFun : public TipType {
    std::vector<TipType*> args;
    TipType *ret;

public:
    TipFun(std::vector<TipType*> args, TipType* ret) : args(args), ret(ret) {}
    std::string tostr() override {
        std::ostringstream oss;
        oss << "(";
        for(TipType* arg : args) {
            oss << arg->tostr() << ",";
        }
        oss << ")" << "->" << ret->tostr();
        return oss.str();
    }
};

