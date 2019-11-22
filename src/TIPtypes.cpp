#include "TIPtypes.h"

bool TIPtype::operator==(const TIPtype& other) const
{
    return toStr() == (&other)->toStr();
}

bool TIPtype::operator!=(const TIPtype& other) const
{
    return toStr() != (&other)->toStr();
}
std::string TIPint::toStr() const
{
    return "int";
};

std::string TIPref::toStr() const
{
    std::ostringstream oss;
    oss << "&" << of->toStr();
    return oss.str();
}

TIPfun::TIPfun(std::vector<TIPtype *> args, TIPtype *ret) : args(args), ret(ret) {}

std::string TIPfun::toStr() const
{
    std::ostringstream oss;
    oss << "(";
    for (TIPtype *arg : args)
    {
        oss << arg->toStr() << ",";
    }
    oss << ")"
        << "->" << ret->toStr();
    return oss.str();
}
