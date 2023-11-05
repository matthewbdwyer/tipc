#pragma once

#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"

enum Optimization {
  licm, del
};

/*! \class Optimizer
 *  \brief routines to optimize generated code.
 */
class Optimizer {
public:
  /*! \brief optimize LLVM module.
   *
   * Apply a series of basic optimization passes to the given LLVM module.
   * \param theModule an LLVM module to be optimized
   * \param enabledOpts the list of enabled optimizations
   */
  static void optimize(llvm::Module *theModule, 
		       llvm::cl::list<Optimization> &enabledOpts);
};
