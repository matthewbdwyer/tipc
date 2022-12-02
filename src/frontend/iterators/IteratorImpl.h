#pragma once
#include "SyntaxTree.h"

/*! \brief The interface that an iterator should follow.
 *
 * Please note that this is not the client-facing interface. This is a tipc internal.
 */
class IteratorImpl {

public:
  explicit IteratorImpl (SyntaxTree const &tree);

  virtual ~IteratorImpl ();

  virtual SyntaxTree operator* () = 0;

  virtual SyntaxTree *operator->() = 0;

  virtual void operator++ () = 0;

  virtual bool operator== (const IteratorImpl &rhs) const = 0;

  virtual bool operator!= (const IteratorImpl &rhs) const = 0;

  virtual IteratorImpl *clone() = 0;

  SyntaxTree const & get_tree() const;


protected:
  SyntaxTree const &tree;

};
