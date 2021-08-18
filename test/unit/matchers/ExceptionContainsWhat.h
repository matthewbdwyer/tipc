#include <catch2/matchers/catch_matchers_exception.hpp>

#include <iostream>
#include <string>

class ExceptionContainsWhat : public Catch::Matchers::MatcherBase<std::exception> {
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
