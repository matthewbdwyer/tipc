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