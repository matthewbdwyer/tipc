#pragma once

#include <exception>
#include <string>

class UnificationError: public std::exception {
public:
    UnificationError(const char * msg);
    virtual const char* what() const throw() override;
private:
    const char * msg;
};


