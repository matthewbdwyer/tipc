#include "Optimizer.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

using namespace llvm;

void Optimizer::optimize(Module* theModule) {
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
    TheFPM->run(fun);
  }
}