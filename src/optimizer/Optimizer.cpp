#include "Optimizer.h"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include "loguru.hpp"

using namespace llvm;

void Optimizer::optimize(Module* theModule) {
  LOG_S(1) << "Optimizing program " << theModule->getName().str();

  // Create a pass manager to simplify generated module
  auto TheFPM = std::make_unique<legacy::FunctionPassManager>(theModule);

  // Promote allocas to registers.
  TheFPM->add(createPromoteMemoryToRegisterPass());

  // Do simple "peephole" optimizations
  TheFPM->add(createInstructionCombiningPass());

  // Reassociate expressions.
  TheFPM->add(createReassociatePass());

  // Eliminate Common SubExpressions.
  TheFPM->add(createGVNPass());

  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->add(createCFGSimplificationPass());

  // initialize and run simplification pass on each function
  TheFPM->doInitialization();
  for (auto &fun : theModule->getFunctionList()) {
    LOG_S(1) << "Optimizing function " << fun.getName().str();

    TheFPM->run(fun);
  }
}
