#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class TIPtype {
public:
    virtual std::string toStr() const = 0;
    bool operator==(const TIPtype& other) const;
};

class TIPint : public TIPtype {
public:
    std::string toStr() const override;
};

class TIPref : public TIPtype {
private:
    TIPtype *of;
public:
    TIPref(TIPtype *of);
    std::string toStr() const override;
};

class TIPfun : public TIPtype {
private:
    std::vector<TIPtype*> args;
    TIPtype *ret;
public:
    TIPfun(std::vector<TIPtype*> args, TIPtype* ret);
    std::string toStr() const override;
};