// NB (nphair): It looks like ANTLR unsets this. Find a better solution.
#define EOF -1

#include <catch2/catch.hpp>

#include <iostream>
#include <string>

class ExceptionContainsWhat : public Catch::MatcherBase<std::exception> {
public:
  ExceptionContainsWhat(std::string const & expected) : expected(expected) {}

  bool match(std::exception const & e) const override {
    std::string s(e.what());
    return s.find(expected) != std::string::npos;
  }

  virtual std::string describe() const override {
    return "expected exception what() message containing \"" + expected + "\".";
  }

private:
  std::string expected;
};

inline ExceptionContainsWhat ContainsWhat(std::string const & expected) {
  return ExceptionContainsWhat(expected);
}
