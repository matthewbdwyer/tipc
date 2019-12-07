#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class TIPTypeError : public std::exception {
    std::string msg;
public:
    TIPTypeError(std::string msg);
    virtual const char* what() const noexcept override;
};

class TIPtype {
public:
    bool composite = false;
    virtual std::string print() const = 0;
};

class TIPint : public TIPtype {
public:
    std::string print() const override;
};

class TIPref : public TIPtype {
public:
    TIPtype *of;
    TIPref(TIPtype *of);
    std::string print() const override;
};

class TIPalpha : public TIPtype {
public:
    static std::string ALPHA;
    std::string print() const override;
};

class TIPfun : public TIPtype {
public:
    std::vector<TIPtype*> param_types;
    TIPtype* ret;
    TIPfun(std::vector<TIPtype*> param_types, TIPtype* ret);
    std::string print() const override;
};