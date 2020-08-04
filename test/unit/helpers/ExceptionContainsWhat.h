#include <string>
#include <iostream>
#include "SemanticError.h"

class ExceptionContainsWhat : public Catch::MatcherBase<std::exception> {
public:
  ExceptionContainsWhat(std::string const & expected) : expected(expected) {}

  bool match(std::exception const & e) const override {
    std::string s(e.what());
    return s.find(expected) != std::string::npos;
  }

  virtual std::string describe() const override {
    return "determine if the exception what() message contains \"" + expected + "\".";
  }

private:
  std::string expected;
};

inline ExceptionContainsWhat ContainsWhat(std::string const & expected) {
  return ExceptionContainsWhat(expected);
}
