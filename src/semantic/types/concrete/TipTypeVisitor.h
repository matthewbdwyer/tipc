#pragma once

#include "Type.h"

/*! \brief Base class for TIP type visitors.
 *
 * The type visitor class abstracts the traversal of an type.  It works
 * in concert with the accept methods of TipType subtypes.  Each of those
 * nodes performs a traversal of its children in order.  This class defines 
 * default behavior for the processing performed when the traversal reaches 
 * a node of a given type.
 *
 * By default the visit method returns true, indicating that the children of
 * the node should also be visited, and the endVisit method does nothing.
 *
 * Subtypes of TipTypeVisitor will selectively override these default methods.
 * Overriding a visit permits a pre-order processing during traversal and
 * overriding endVisit permits a post-order processing.
 *
 * Subtype of TipTypeVisitor will also define member data that can be 
 * referenced by overridden methods to communicate information along 
 * the traversal to future method invocations.
 */
class TipTypeVisitor {
public:
  virtual bool visit(TipAlpha * element) { return true; }
  virtual void endVisit(TipAlpha * element) {}
  virtual bool visit(TipFunction * element) { return true; }
  virtual void endVisit(TipFunction * element) {}
  virtual bool visit(TipInt * element) { return true; }
  virtual void endVisit(TipInt * element) {}
  virtual bool visit(TipMu * element) { return true; }
  virtual void endVisit(TipMu * element) {}
  virtual bool visit(TipRecord * element) { return true; }
  virtual void endVisit(TipRecord * element) {}
  virtual bool visit(TipAbsentField * element) { return true; }
  virtual void endVisit(TipAbsentField * element) {}
  virtual bool visit(TipRef * element) { return true; }
  virtual void endVisit(TipRef * element) {}
  virtual bool visit(TipVar * element) { return true; }
  virtual void endVisit(TipVar * element) {}
};

