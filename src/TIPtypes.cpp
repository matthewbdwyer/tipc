#include "TIPtypes.h"

TIPTypeError::TIPTypeError(std::string msg) : msg(msg) {}
const char* TIPTypeError::what() const noexcept {
    return msg.c_str();
}

std::string TIPint::print() const
{
    return "int";
};

TIPref::TIPref(TIPtype *of) : of(of) {}

std::string TIPref::print() const
{
    std::ostringstream oss;
    oss << "&" << of->print();
    return oss.str();
}

std::string TIPalpha::ALPHA = "α";

std::string TIPalpha::print() const 
{
    return ALPHA;
}

TIPfun::TIPfun(std::vector<TIPtype*> params_type, TIPtype* ret) : params_type(params_type), ret(ret){
    this->composite = true;
}

std::string TIPfun::print() const 
{
    std::string fun_type = "(";
    for (auto it = params_type.begin(); it != params_type.end(); it++) {
        fun_type += (*it)->print();
        if (std::next(it) != params_type.end()) {
            fun_type += ",";
        }
    }
    fun_type += ") -> ";
    fun_type += ret->print();
    return fun_type;
}