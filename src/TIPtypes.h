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
    virtual std::string print() const = 0;
};

class TIPint : public TIPtype {
public:
    std::string print() const override;
};

class TIPref : public TIPtype {
private:
    TIPtype *of;
public:
    TIPref(TIPtype *of);
    std::string print() const override;
};

class TIPfun : public TIPtype {
private:
    std::vector<TIPtype*> args;
    TIPtype *ret;
public:
    TIPfun(std::vector<TIPtype*> args, TIPtype* ret);
    std::string print() const override;
};