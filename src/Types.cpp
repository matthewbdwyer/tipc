#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class TipType {
public:
    virtual std::string print() = 0;
};

class TipInt : public TipType {
public:
    std::string print() override {
        return "int";
    }
};

class TipRef : public TipType {
    TipType *of;

public:
    TipRef(TipType *of) : of(of) {}

    std::string print() override {
        std::ostringstream oss;
        oss << "&" << of->print();
        return oss.str();
    }
};

class TipFun : public TipType {
    std::vector<TipType*> args;
    TipType *ret;

public:
    TipFun(std::vector<TipType*> args, TipType* ret) : args(args), ret(ret) {}
    std::string print() override {
        std::ostringstream oss;
        oss << "(";
        for(TipType* arg : args) {
            oss << arg->print() << ",";
        }
        oss << ")" << "->" << ret->print();
        return oss.str();
    }
};

// // test below
// int main(void) {
//     TipInt *tip_int = new TipInt();
//     TipRef *tip_ref = new TipRef(new TipInt());
//     std::vector<TipType*> vec;
//     vec.push_back(new TipInt());
//     vec.push_back(new TipRef(new TipInt()));
//     TipFun *tip_fun = new TipFun(vec, new TipInt());
//     std::cout << tip_int->print() << std::endl;
//     std::cout << tip_ref->print() << std::endl;
//     std::cout << tip_fun->print() << std::endl;
//     return 0;
// }

