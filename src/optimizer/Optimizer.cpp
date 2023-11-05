#include "Optimizer.h"

// New PassBuilder
#include "llvm/Passes/PassBuilder.h"

// New Passes
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"

// P5 passes
#include "llvm/Transforms/Scalar/LICM.h"
#include "llvm/Transforms/Scalar/LoopDeletion.h"

// For logging
#include "loguru.hpp"

namespace { // Anonymous namespace for local function
	    
bool contains(Optimization o, llvm::cl::list<Optimization> &l) {
  for (unsigned i = 0; i != l.size(); ++i) {
    if (o == l[i]) return true;
  }
  return false;
}

}

//  NOTE:
//  We must use llvm Adaptors to run per-loop passes in the function pass
//  manager. In LLVM14+, The hierarchy for code sections is : Module -> (CGSCC)*
//  -> Functions -> Loops
//
//  [*] is optional.
//
//  eg: To run a loop pass on a module ->
//  ModulePassManager.add(functionAdaptor(LoopAdaptor(llvm::LoopPass())))

void Optimizer::optimize(llvm::Module *theModule, 
		llvm::cl::list<Optimization> &enabledOpts) {
  LOG_S(1) << "Optimizing program " << theModule->getName().str();

  // New pass builder

  llvm::PassBuilder passBuilder;

  // Setting-up Analysis Managers

  llvm::FunctionAnalysisManager functionAnalysisManager;
  llvm::ModuleAnalysisManager moduleAnalysisManager;
  llvm::LoopAnalysisManager loopAnalysisManager;
  llvm::CGSCCAnalysisManager cgsccAnalysisManager;

  // Registering the analysis managers with the pass builder

  passBuilder.registerModuleAnalyses(moduleAnalysisManager);
  passBuilder.registerCGSCCAnalyses(cgsccAnalysisManager);
  passBuilder.registerFunctionAnalyses(functionAnalysisManager);
  passBuilder.registerLoopAnalyses(loopAnalysisManager);
  // Cross Register Proxies
  passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager,
                                   cgsccAnalysisManager, moduleAnalysisManager);

  // Initiating Function and Module level PassManagers

  llvm::ModulePassManager modulePassManager;
  llvm::FunctionPassManager functionPassManager;

  // Adding passes to the pipeline

  functionPassManager.addPass(llvm::PromotePass()); // New Reg2Mem
  functionPassManager.addPass(llvm::InstCombinePass());
  // Reassociate expressions.
  functionPassManager.addPass(llvm::ReassociatePass());
  // Eliminate Common SubExpressions.
  functionPassManager.addPass(llvm::GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  functionPassManager.addPass(llvm::SimplifyCFGPass());

  if (contains(licm, enabledOpts)) {
    // Add loop invariant code motion with option to use MemorySSA
    functionPassManager.addPass(
        llvm::createFunctionToLoopPassAdaptor(llvm::LICMPass(), true)); 
  }

  if (contains(del, enabledOpts)) {
    // Add loop deletion pass
    functionPassManager.addPass(
        llvm::createFunctionToLoopPassAdaptor(llvm::LoopDeletionPass())); 
  }   

  // Passing the function pass manager to the modulePassManager using a function
  // adaptor, then passing theModule to the ModulePassManager along with
  // ModuleAnalysisManager.

  modulePassManager.addPass(
      createModuleToFunctionPassAdaptor(std::move(functionPassManager)));
  modulePassManager.run(*theModule, moduleAnalysisManager);
}
