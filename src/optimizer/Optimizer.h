#pragma once

#include "llvm/IR/Module.h"
#include <map>

/*! \class Optimizer
 *  \brief routines to optimize generated code.
 */
class Optimizer {
public:
  //! \brief keys for optimization passes that can be disabled
  enum DisoptPass { all, pmr, ic, re, gvn, cfgs, tce, NumPasses };

  //! \brief strings for optimization pass keys
  static const constexpr char* const cmdLine[NumPasses] = {
    "disable all optimizations",
    "disable promote memory to register",
    "disable instruction combining",
    "disable reassociation",
    "disable global value numbering",
    "disable CFG simplification",
    "disable tail call elimination"
  };

  /*! \brief optimize LLVM module. 
   *
   * Apply a series of basic optimization passes to the given LLVM module.
   * \param theModule an LLVM module to be optimized
   * \param pass an LLVM optimization pass to disable
   */
  static void optimize(llvm::Module* theModule, DisoptPass pass);
};
