#include "Optimizer.h"
#include "llvm/IR/PassManager.h" // change from #include "llvm/IR/LegacyPassManager.h" to new pass manager
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h" // to include instruction combine pass
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/NewGVN.h" // to include GVN pass
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/InstSimplifyPass.h" // to include InstSimplifyPass pass
using namespace llvm;

void Optimizer::optimize(Module* theModule)
{
    // Create a pass manager to simplify generated module
    FunctionPassManager FPM(true);

    FPM.addPass(PromotePass());

    // Do simple "peephole" optimizations
    FPM.addPass(InstCombinePass());  //InstCombinePass doesn't work

    // Reassociate expressions.
     FPM.addPass(ReassociatePass()); //ReassociatePass doesn't work

    // Eliminate Common SubExpressions.
    FPM.addPass(NewGVNPass());

    // Simplify the control flow graph (deleting unreachable blocks, etc).
    FPM.addPass(SimplifyCFGPass());

    FunctionAnalysisManager FAM;
    PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);

    //run simplification pass on each function

    for (auto& fun : theModule->getFunctionList()) {

        std::string fun_name = fun.getName().str();

//skipped to these method bcz of segmentation faults
        if (fun_name.compare("llvm.donothing") == 0 || fun_name.compare("calloc") == 0 || fun_name.compare("_tip_error") == 0 || fun_name.compare("_tip_main_undefined") == 0 || 			  	fun_name.compare("_tip_output") == 0 || fun_name.compare("_tip_input") == 0) continue;

        FPM.run(fun, FAM);
    }
}

