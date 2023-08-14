#include "Optimizer.h"

//New PassBuilder
#include "llvm/Passes/PassBuilder.h"

// New Passes
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Transforms/Scalar/GVN.h"


//Extra Passes
#include "llvm/Transforms/Scalar/SCCP.h"
#include "llvm/Transforms/Scalar/TailRecursionElimination.h" //Tail Call Elimination
#include "llvm/Transforms/Scalar/SimpleLoopUnswitch.h"


//For logging
#include "loguru.hpp"


//  NOTE:
//  We must use llvm Adaptors to run per-loop passes in the function pass manager.
//  In LLVM14+, The hierarchy for code sections is :
//  Module -> (CGSCC)* -> Functions -> Loops
//  
//  [*] is optional.
//  
//  eg: To run a loop pass on a module -> ModulePassManager.add(functionAdaptor(LoopAdaptor(llvm::LoopPass())))



void Optimizer::optimize(llvm::Module *theModule) {
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
  passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager, cgsccAnalysisManager,moduleAnalysisManager);
  
  // Initiating Function and Module level PassManagers
    
  llvm::ModulePassManager modulePassManager;
  llvm::FunctionPassManager functionPassManager;
  
  // Adding passes to the pipeline

  functionPassManager.addPass(llvm::PromotePass()); //New Reg2Mem
  functionPassManager.addPass(llvm::InstCombinePass());
  // Reassociate expressions.
  functionPassManager.addPass(llvm::ReassociatePass());
  // Eliminate Common SubExpressions.
  functionPassManager.addPass(llvm::GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  functionPassManager.addPass(llvm::SimplifyCFGPass());
  
  // Passing the function pass manager to the modulePassManager using a function adaptor, then passing theModule to the ModulePassManager along with ModuleAnalysisManager.
   
  modulePassManager.addPass(createModuleToFunctionPassAdaptor(std::move(functionPassManager)));
  modulePassManager.run(*theModule, moduleAnalysisManager);

}
