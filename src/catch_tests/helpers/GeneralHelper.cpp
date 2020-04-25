#include "GeneralHelper.h"

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
