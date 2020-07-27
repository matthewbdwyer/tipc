#pragma once 

#include "ASTNode.h"

#include <algorithm>

/*
 * Convert a vector of unique ptrs to pointers.
 * This is used when the client does not need ownership, 
 * e.g., they wish only to read the pointer.
 */
template<typename T>
std::vector<T*> rawRefs(std::vector<std::unique_ptr<T>> &v) {
  std::vector<T*> r;
  std::transform(v.begin(), v.end(), 
                 std::back_inserter(r), 
                 [](auto& up){return up.get();});
  return r;
}
