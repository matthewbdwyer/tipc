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

void Optimizer::optimize(Module* theModule, DisoptPass pass) {
  LOG_S(1) << "Optimizing program " << theModule->getName().str();

  if (pass == Optimizer::DisoptPass::all) return;

  // Create a pass manager to simplify generated module
  auto TheFPM = std::make_unique<legacy::FunctionPassManager>(theModule);

  // Promote allocas to registers.
  if (pass != Optimizer::DisoptPass::pmr) {
    LOG_S(1) << "Enabling Promote Memory to Register Pass";
    TheFPM->add(createPromoteMemoryToRegisterPass());
  }

  // Do simple "peephole" optimizations
  if (pass != Optimizer::DisoptPass::ic) {
    LOG_S(1) << "Enabling Instruction Combining Pass";
    TheFPM->add(createInstructionCombiningPass());
  }

  // Reassociate expressions.
  if (pass != Optimizer::DisoptPass::re) {
    LOG_S(1) << "Enabling Reassociation Pass";
    TheFPM->add(createReassociatePass());
  }

  // Eliminate Common SubExpressions.
  if (pass != Optimizer::DisoptPass::gvn) {
    LOG_S(1) << "Enabling Elimination of Common Subexpression Pass";
    TheFPM->add(createGVNPass());
  }

  // Simplify the control flow graph (deleting unreachable blocks, etc).
  if (pass != Optimizer::DisoptPass::cfgs) {
    LOG_S(1) << "Enabling Simplify Control Flow Graph Pass";
    TheFPM->add(createCFGSimplificationPass());
  }

  // Eliminate tail calls
  if (pass != Optimizer::DisoptPass::tce) {
    LOG_S(1) << "Enabling Tail Call Elimination Pass";
    TheFPM->add(createTailCallEliminationPass());
  }

  // initialize and run simplification pass on each function
  TheFPM->doInitialization();
  for (auto &fun : theModule->getFunctionList()) {
    LOG_S(1) << "Optimizing function " << fun.getName().str();

    TheFPM->run(fun);
  }
}
