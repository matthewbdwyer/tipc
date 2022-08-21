#include "IteratorImpl.h"

namespace IteratorUtils {

  /*! \fn are_equal
   *  \brief A templated utility to check if to iterators are logically equal.
   *
   *  Equality means of the same type and at the same point in the traversal process.
   */
  template<typename T>
  bool are_equal(T *lhs, const IteratorImpl &rhs) {
    // Different iterator implementations cannot be equal.
    auto rhs_downcast = dynamic_cast<decltype(lhs)>(&rhs);
    if(rhs_downcast == nullptr) {
      return false;
    }

    // If the underlying trees roots are different then the iterators are not equal.
    SyntaxTree const &lhs_tree = lhs->get_tree();
    SyntaxTree const &rhs_tree = rhs_downcast->get_tree();
    if(lhs_tree.getRoot() != rhs_tree.getRoot()) {
      return false;
    }

    // If we have processed a different number of nodes these iterators cannot be at the same point and are not equal.
    // NB: Fragile, there is no guarantee an implementation has a stack.
    std::stack<SyntaxTree> const &lhs_stack = lhs->stack;
    std::stack<SyntaxTree> const &rhs_stack = rhs_downcast->stack;
    if(lhs_stack.size() != rhs_stack.size()) {
      return false;
    }

    if(lhs_stack.empty() && rhs_stack.empty()) {
      return true;
    }

    return lhs_stack.top().getRoot() == rhs_stack.top().getRoot();
  }

}
