#pragma once 

#include "ASTNode.h"

#include <algorithm>

/*! \fn rawRefs
 *  \brief Convert a vector of unique ptrs to pointers.
 * 
 * This is used when the client does not need ownership, 
 * e.g., they wish only to read the pointer.
 * \param v a vector of unique pointers.
 * \return a vector of the underlying pointers.
 */
template<typename T>
std::vector<T*> rawRefs(const std::vector<std::unique_ptr<T>> &v) {
  std::vector<T*> r;

  /*
   * This call passes an anonymous function, i.e., a lambda, that is
   * applied to each element in the input vector.   The result of that
   * application is used to build up the output vector.  If you haven't
   * seen lambda's before in C++ they are denoted by the "[]" syntax.
   */
  std::transform(v.begin(), v.end(), 
                 std::back_inserter(r), 
                 [](auto& up){return up.get();});
  return r;
}

template<typename T>
std::vector<T*> rawRefs(const std::vector<std::shared_ptr<T>> &v) {
  std::vector<T*> r;

  /*
   * This call passes an anonymous function, i.e., a lambda, that is
   * applied to each element in the input vector.   The result of that
   * application is used to build up the output vector.  If you haven't
   * seen lambda's before in C++ they are denoted by the "[]" syntax.
   */
  std::transform(v.begin(), v.end(),
                 std::back_inserter(r),
                 [](auto& up){return up.get();});
  return r;
}
