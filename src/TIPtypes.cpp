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

TIPfun::TIPfun(std::vector<TIPtype *> args, TIPtype *ret) : args(args), ret(ret) {}

std::string TIPfun::print() const
{
    std::ostringstream oss;
    oss << "(";
    for (auto it = args.begin(); it != args.end(); ++it)
    {
        oss << (*it)->print();
        if (std::next(it) != args.end())
        {
            oss << ",";
        }
    }
    oss << ")" << "->" << ret->print();
    return oss.str();
}

