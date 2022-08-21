#include "GeneralHelper.h"
#include <cstddef>

std::vector<std::string> GeneralHelper::tokenize(std::string str, char delim) {
  std::vector<std::string> tokens;

  int startIndex = 0;
  int endIndex = 0;
  while ((endIndex = str.find(delim, startIndex)) != std::string::npos) {
    tokens.push_back(str.substr(startIndex, endIndex-startIndex));
    startIndex = endIndex + 1;
  }
  tokens.push_back(str.substr(startIndex));
  return tokens;
}

std::string GeneralHelper::removeTrailingWhitespace(std::string str) {
  std::string strNoWhiteSpace = "";
  std::string delimeter = "\n";
  const std::string WHITESPACE = " \t\f\v";
  size_t pos = 0;
  while ((pos = str.find(delimeter)) != std::string::npos) {
    std::string newLine = str.substr(0, pos);
    size_t end = newLine.find_last_not_of(WHITESPACE);
    newLine = (end == std::string::npos) ? "" : newLine.substr(0, end + 1);
    strNoWhiteSpace += newLine + "\n";
    str.erase(0, pos + delimeter.length());
  }

  return strNoWhiteSpace;
}

int GeneralHelper::countSubstrings(std::string str, std::string substr) {
  int count = 0;
  std::string::size_type index = 0;
  while((index = str.find(substr, index)) != std::string::npos) {
    count++;
    index += substr.length();
  }
  return count;
}
