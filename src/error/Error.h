#pragma once

#include <exception>
#include <string>
#include <iostream>

/*! \brief Base class for tipc errors.
 *
 * Errors detected during compilation result in exceptions being thrown.
 * The exception carries a message -- the return value of the what() method.
 */
class Error: public std::exception {
public:
  Error(const std::string & msg) : msg(msg) {} //! Construct error with message
  virtual const char* what() const noexcept; //! Returns message
private:
  const std::string msg;
};


