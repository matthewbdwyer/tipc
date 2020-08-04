#pragma once

#include "llvm/IR/Module.h"

/*! \class Optimizer
 *  \brief routines to optimize generated code.
 */
class Optimizer {
public:

  /*! \brief optimize LLVM module. 
   *
   * Apply a series of basic optimization passes to the given LLVM module.
   * \param theModule an LLVM module to be optimized
   */
  static void optimize(llvm::Module* theModule);
};
