#include "CodeGenerator.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

using namespace AST;

/*
 * These code generation routines are based on the LLVM Kaleidoscope
 * Tutorial for C++ Chapters 3 and 7
 *     https://llvm.org/docs/tutorial/LangImpl03.html
 *     https://llvm.org/docs/tutorial/LangImpl07.html
 * We thank the authors for making these example available.
 *
 * Unlike the tutorial, these routines rely on following LLVM passes
 * to transform the code to SSA form, via the PromoteMemToReg pass,
 * and especially the CFGSimplification pass, which removes nops and
 * dead blocks.
 */

/********* data structures for storing code gen information ********/

// TBD: these should really be member data of the visitor, but
// doing that (a) exposes the implementation and (b) makes for
// some complex constructor initialization.

/*
 * These structures records lots of information that spans a code
 * gen pass.  For example, the current insertion, entry, return blocks
 * in the current function.
 */
static LLVMContext Context;
static IRBuilder<> Builder(Context);

// Passes value up the visitor; this is a dummy initial value
static Value *codegenValue = ConstantInt::get(Type::getInt64Ty(Context), 0);

// Permits getFunction to access the current module being compiled
static std::unique_ptr<Module> CurrentModule = nullptr;

/*
 * This function symbol table stores, for each function, its index
 * and the names of its formal parameters.
 *
 * TBD: It currently relies on the fact that all function parameters are integer
 * type, so we don't need to indicate the type explicitly.  Ultimately, we will
 * want to record pairs of formals and types here.
 */
static std::map<std::string, std::pair<int, std::vector<std::string>>> FunctionDecls;

/*
 * This structure stores the mapping from names in a function scope
 * to their LLVM values.  The structure is built when entering a
 * scope and cleared when exiting a scope.
 */
static std::map<std::string, AllocaInst *> NamedValues;

/*
 * An intrinsic function in a program representation provides the declaration
 * for a function that is defined "elsewhere", e.g., hardwired into code
 * generation, in a runtime library, in special hardware support, etc.
 *
 * For TIP we use intrinsics for several purposes.  To construct a "nop",
 * using an LLVM intrinsic, to perform TIP specific IO, and and
 * to allocate heap memory.
 */
static llvm::Function *nop = nullptr;
static llvm::Function *inputIntrinsic = nullptr;
static llvm::Function *outputIntrinsic = nullptr;
static llvm::Function *errorIntrinsic = nullptr;
static llvm::Function *mallocFun = nullptr;

// A counter to create unique labels
static int labelNum = 0;

// Indicate whether the expression code gen is for an L-value
static bool lValueGen = false;

/*
 * The global function dispatch table is created in a shallow pass over
 * the function signatures, stored here, and then referenced in generating
 * function applications.
 */
static GlobalVariable *tipFTable = nullptr;

// The number of TIP program parameters
static int numTIPArgs = 0;

/*
 * The global argument count and array are used to communicate command
 * line inputs to the TIP main function.
 */
static GlobalVariable *tipNumInputs = nullptr;
static GlobalVariable *tipInputArray = nullptr;

/********************* code gen routines ************************/

std::unique_ptr<llvm::Module> CodeGenerator::codegen(
          std::unique_ptr<AST::Program> p, std::string programName) {
  CodeGenerator visitor;
  p->setName(programName);
  p->accept(&visitor);
  return std::move(CurrentModule);
}

void CodeGenerator::visit(AST::Program *n) {
  // Create module to hold generated code
  auto TheModule = std::make_unique<Module>(n->getName(), Context);

  // Initialize nop declaration
  nop = Intrinsic::getDeclaration(TheModule.get(), Intrinsic::donothing);

  labelNum = 0;

  // Transfer the module for access by shared codegen routines
  CurrentModule = std::move(TheModule);

  /*
   * This shallow pass over the function declarations builds the
   * function symbol table, creates the function declarations, and
   * builds the function dispatch table.
   */
  {
    /*
     * First create the local function symbol table which stores
     * the function index and formal parameters
     */
    int funIndex = 0;
    for (auto const &fn : n->FUNCTIONS) {
      std::pair<int, std::vector<std::string>> thePair(funIndex++,
                                                       fn->getFormals());
      FunctionDecls[fn->getName()] = thePair;
    }

    /*
     * Create the llvm functions.
     * Store as a vector of constants, which works because Function
     * is a subtype of Constant, to workaround the inability of the
     * compiler to find a conversion from Function to Constant
     * below in creating the ftableInit.
     */
    std::vector<llvm::Constant *> programFunctions;
    for (auto const &fn : n->FUNCTIONS) {
      programFunctions.push_back(getFunction(fn->getName()));
    }

    /*
     * Create a generic function pointer type "() -> Int64" that is
     * used to declare the global function dispatch table and to
     * bitcast the declared functions prior to inserting them.
     */
    auto *genFunPtrType = PointerType::get(
        FunctionType::get(Type::getInt64Ty(Context), None, false), 0);

    // Create and record the function dispatch table
    auto *ftableType = ArrayType::get(genFunPtrType, funIndex);

    // Cast TIP functions to the generic function pointer type for initializer
    std::vector<Constant *> castProgramFunctions;
    for (auto const &pf : programFunctions) {
      castProgramFunctions.push_back(
          ConstantExpr::getPointerCast(pf, genFunPtrType));
    };

    /*
     * Create initializer for function table using generic function type
     * and set the initial value.
     */
    auto *ftableInit = ConstantArray::get(ftableType, castProgramFunctions);

    // Create the global function dispatch table
    tipFTable = new GlobalVariable(*CurrentModule, ftableType, true,
                                   llvm::GlobalValue::InternalLinkage,
                                   ftableInit, "_tip_ftable");
  }

  /*
   * Generate globals that establish the parameter passing structures from the
   * rtlib main() and define a "_tip_main" if one is not already defind.
   */
  {
    /*
     * If there is no "main(...)" defined in this TIP program we
     * create main that calls the "_tip_main_undefined()" rtlib function.
     *
     * For this function we perform all code generation here and
     * we never visit it during the code gen traversals - since
     * the function doesn't exist in the TIP program.
     */
    auto fd = FunctionDecls.find("main");
    if (fd == FunctionDecls.end()) {
      auto *M = llvm::Function::Create(
          FunctionType::get(Type::getInt64Ty(Context), false),
          llvm::Function::ExternalLinkage, "_tip_main", CurrentModule.get());
      BasicBlock *BB = BasicBlock::Create(Context, "entry", M);
      Builder.SetInsertPoint(BB);

      auto *undef = llvm::Function::Create(
          FunctionType::get(Type::getVoidTy(Context), false),
          llvm::Function::ExternalLinkage, "_tip_main_undefined",
          CurrentModule.get());
      Builder.CreateCall(undef);
      Builder.CreateRet(ConstantInt::get(Type::getInt64Ty(Context), 0));
    }

    // create global _tip_num_inputs with init of numTIPArgs
    tipNumInputs = new GlobalVariable(
        *CurrentModule, Type::getInt64Ty(Context), true,
        llvm::GlobalValue::ExternalLinkage,
        ConstantInt::get(Type::getInt64Ty(Context), numTIPArgs),
        "_tip_num_inputs");

    // create global _tip_input_array with up to numTIPArgs of Int64
    auto *inputArrayType =
        ArrayType::get(Type::getInt64Ty(Context), numTIPArgs);
    std::vector<Constant *> zeros(
        numTIPArgs, ConstantInt::get(Type::getInt64Ty(Context), 0));
    tipInputArray = new GlobalVariable(
        *CurrentModule, inputArrayType, false, llvm::GlobalValue::CommonLinkage,
        ConstantArray::get(inputArrayType, zeros), "_tip_input_array");
  }

  // Code is generated into the module by the other routines
  for (auto const &fn : n->FUNCTIONS) {
    fn->accept(this);
  }

  verifyModule(*CurrentModule);
}

void CodeGenerator::visit(AST::Function *n) {
  llvm::Function *TheFunction = getFunction(n->getName());
  if (TheFunction == nullptr) {
    return;
  }

  // create basic block to hold body of function definition
  BasicBlock *BB = BasicBlock::Create(Context, "entry", TheFunction);
  Builder.SetInsertPoint(BB);

  // keep scope separate from prior definitions
  NamedValues.clear();

  /*
   * Add arguments to the symbol table
   *   - for main function, we initialize allocas with array loads
   *   - for other functions, we initialize allocas with the arg values
   */
  if (n->getName() == "main") {
    int argIdx = 0;
    // Note that the args are not in the LLVM function decl, so we use the AST
    // formals
    for (auto &argName : n->getFormals()) {
      // Create an alloca for this argument and store its value
      AllocaInst *argAlloc = CreateEntryBlockAlloca(TheFunction, argName);

      // Emit the GEP instruction to index into input array
      std::vector<Value *> indices;
      indices.push_back(ConstantInt::get(Type::getInt64Ty(Context), 0));
      indices.push_back(ConstantInt::get(Type::getInt64Ty(Context), argIdx));
      auto *gep = 
          Builder.CreateInBoundsGEP(tipInputArray, indices, "inputidx");

      // Load the value and store it into the arg's alloca
      auto *inVal =
          Builder.CreateLoad(gep, "tipinput" + std::to_string(argIdx++));
      Builder.CreateStore(inVal, argAlloc);

      // Record name binding to alloca
      NamedValues[argName] = argAlloc;
    }
  } else {
    for (auto &arg : TheFunction->args()) {
      // Create an alloca for this argument and store its value
      AllocaInst *argAlloc = CreateEntryBlockAlloca(TheFunction, arg.getName());
      Builder.CreateStore(&arg, argAlloc);

      // Record name binding to alloca
      NamedValues[arg.getName()] = argAlloc;
    }
  }

  bool success = true;

  // add local declarations to the symbol table
  for (auto const &decl : n->DECLS) {
    decl->accept(this);
    success |= (codegenValue != nullptr);
  }

  for (auto &stmt : n->BODY) {
    stmt->accept(this);
    success |= (codegenValue != nullptr);
  }

  if (success) {
    // internal LLVM helper function to detect errors in function defs
    verifyFunction(*TheFunction);
  } else {
    // error in generating body, so remove partial function definition
    TheFunction->eraseFromParent();
  }
}

/* 
 * These routines are expression visit methods
 * As such the member data of the code generator visitor is directly accessible.
 */

void CodeGenerator::visit(AST::NumberExpr *n) {
  codegenValue = ConstantInt::get(Type::getInt64Ty(Context), n->VAL);
}

void CodeGenerator::visit(AST::BinaryExpr *n) {
  n->LHS->accept(this);
  Value *L = codegenValue;

  n->RHS->accept(this);
  Value *R = codegenValue;

  if (L == nullptr || R == nullptr) {
    codegenValue = nullptr;
    return;
  }

  if (n->OP == "+") {
    codegenValue = Builder.CreateAdd(L, R, "addtmp");
  } else if (n->OP == "-") {
    codegenValue = Builder.CreateSub(L, R, "subtmp");
  } else if (n->OP == "*") {
    codegenValue = Builder.CreateMul(L, R, "multmp");
  } else if (n->OP == "/") {
    codegenValue = Builder.CreateSDiv(L, R, "divtmp");
  } else if (n->OP == ">") {
    codegenValue = Builder.CreateICmpSGT(L, R, "gttmp");
  } else if (n->OP == "==") {
    codegenValue = Builder.CreateICmpEQ(L, R, "eqtmp");
  } else {
    codegenValue = LogError("Invalid binary operator: " + n->OP);
  }
}

/*
 * First lookup the variable in the symbol table for names and
 * if that fails, then look in the symbol table for functions.
 *
 * This relies on the fact that TIP programs do not permit duplicate names.
 */
void CodeGenerator::visit(AST::VariableExpr *n) {
  auto nv = NamedValues.find(n->NAME);

  // Lookup name in variable symbol table
  if (nv != NamedValues.end()) {
    if (lValueGen) {
      lValueGen = false;
      codegenValue = NamedValues[nv->first];
    } else {
      codegenValue = Builder.CreateLoad(nv->second, n->NAME.c_str());
    }
    return;
  }

  // Lookup name in variable function symbol table
  auto fd = FunctionDecls.find(n->NAME);
  if (fd == FunctionDecls.end()) {
    codegenValue = LogError("Unknown variable name: " + n->NAME);
  }

  codegenValue = ConstantInt::get(Type::getInt64Ty(Context), (fd->second).first);
}

void CodeGenerator::visit(AST::InputExpr *n) {
  if (inputIntrinsic == nullptr) {
    auto *FT = FunctionType::get(Type::getInt64Ty(Context), false);
    inputIntrinsic = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                            "_tip_input", CurrentModule.get());
  }
  codegenValue = Builder.CreateCall(inputIntrinsic);
}

/*
 * Function application in TIP can either be through explicitly named
 * functions or through expressions that evaluate to a function reference.
 * We consolidate these two cases by binding function names to values
 * and then using those values, which may flow through the program as function
 * references, to index into a function dispatch table to invoke the function.
 *
 * The function name values and table are setup in a shallow-pass over
 * functions performed during codegen for the Program.
 */
void CodeGenerator::visit(AST::FunAppExpr *n) {
  /*
   * Evaluate the function expression - it will resolve to an integer value
   * whether it is a function literal or an expression.
   */
  n->FUN->accept(this);
  if (codegenValue == nullptr) return;
  auto *funVal = codegenValue;

  /*
   * Emit the GEP instruction to compute the address of LLVM function
   * pointer to be called.
   */
  std::vector<Value *> indices;
  indices.push_back(ConstantInt::get(Type::getInt64Ty(Context), 0));
  indices.push_back(funVal);
  auto *gep = Builder.CreateInBoundsGEP(tipFTable, indices, "ftableidx");

  // Load the function pointer
  auto *genericFunPtr = Builder.CreateLoad(gep, "genfptr");

  /*
   * Compute the specific function pointer type based on the actual parameter
   * list.
   *
   * TBD: Currently the type is Int64^N -> Int64,
   * where N is the length of the list
   *
   * Once type information is available we will need to iterate the actuals
   * and construct the per actual vector of types.
   */
  std::vector<Type *> actualTypes(n->ACTUALS.size(), Type::getInt64Ty(Context));
  auto *funPtrType = PointerType::get(
      FunctionType::get(Type::getInt64Ty(Context), actualTypes, false), 0);

  // Bitcast the function pointer to the call-site determined function type
  auto *castFunPtr =
      Builder.CreatePointerCast(genericFunPtr, funPtrType, "castfptr");

  // Compute the actual parameters
  std::vector<Value *> argsV;
  for (auto const &arg : n->ACTUALS) {
    arg->accept(this);
    if (codegenValue == nullptr) return;
    Value *ArgVal = codegenValue;
    argsV.push_back(ArgVal);
  }

  codegenValue = Builder.CreateCall(castFunPtr, argsV, "calltmp");
}

void CodeGenerator::visit(AST::AllocExpr *n) {
  n->ARG->accept(this);
  if (codegenValue == nullptr) return;
  Value *argVal = codegenValue;

  if (mallocFun == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(Context));
    auto *FT = FunctionType::get(Type::getInt8PtrTy(Context), oneInt, false);
    mallocFun = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                       "malloc", CurrentModule.get());
    mallocFun->addFnAttr(llvm::Attribute::NoUnwind);
    mallocFun->addAttribute(0, llvm::Attribute::NoAlias);
  }

  // Since we do not support records all allocs are for 8 bytes, i.e., int64_t
  std::vector<Value *> oneArg(
      1, ConstantInt::get(Type::getInt64Ty(Context), 8));
  auto *allocInst = Builder.CreateCall(mallocFun, oneArg, "allocPtr");
  auto *castPtr = Builder.CreatePointerCast(
      allocInst, Type::getInt64PtrTy(Context), "castPtr");
  // Initialize with argument
  auto *initializingStore = Builder.CreateStore(argVal, castPtr);

  codegenValue = Builder.CreatePtrToInt(castPtr, Type::getInt64Ty(Context),
                                "allocIntVal");
}

void CodeGenerator::visit(AST::NullExpr *n) {
  auto *nullPtr = ConstantPointerNull::get(Type::getInt64PtrTy(Context));
  codegenValue = Builder.CreatePtrToInt(nullPtr, Type::getInt64Ty(Context),
                                "nullPtrIntVal");
}

/* '&' address of expression
 *
 * Only variables can serve as arguments to this operator.
 * Our code generation strategy has allocated all such variables
 * on the stack (via "alloca").  Consequently, in llvm this means
 * that the variable holds the address of stack location, but
 * we explicitly cast it with "ptrtoint" to enforce our invariant
 * that all code generation routines produce int values.
 */
void CodeGenerator::visit(AST::RefExpr *n) {
  Value *argVal = NamedValues[n->NAME];
  if (argVal == nullptr) {
    codegenValue = LogError("Unknown variable name: " + n->NAME);
  }

  codegenValue = Builder.CreatePtrToInt(argVal, Type::getInt64Ty(Context),
                                "intPtrVal");
}

/* '*' dereference expression
 *
 * The argument is assumed to be a reference expression, but
 * our code generation strategy stores everything as an integer.
 * Consequently, we convert the value with "inttoptr" before loading
 * the value at the pointed-to memory location.
 */
void CodeGenerator::visit(AST::DeRefExpr *n) {
  if (lValueGen) {
    /* 
     * For an l-value, just compute the address of the outermost expression
     * all sub-expressions are actually r-values.
     */
    lValueGen = false;

    n->ARG->accept(this);
    if (codegenValue == nullptr) return;
    Value *argVal = codegenValue;

    codegenValue = Builder.CreateIntToPtr(argVal, Type::getInt64PtrTy(Context),
                                  "ptrIntVal");

  } else {
    // For an r-value, compute the address and return the value it points to.
    n->ARG->accept(this);
    if (codegenValue == nullptr) return;
    Value *argVal = codegenValue;

    auto *ref = Builder.CreateIntToPtr(argVal, Type::getInt64PtrTy(Context),
                                       "ptrIntVal");
    codegenValue = Builder.CreateLoad(ref, "valueAt");
  }
}

void CodeGenerator::visit(AST::RecordExpr *n) {
  codegenValue = LogError("Record expressions not implemented");
}

void CodeGenerator::visit(AST::FieldExpr *n) {
  codegenValue = LogError("Field expressions not implemented");
}

void CodeGenerator::visit(AST::AccessExpr *n) {
  codegenValue = LogError("Field access expressions not implemented");
}

void CodeGenerator::visit(AST::DeclStmt *n) {
  // The LLVM builder records the function we are currently generating
  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  AllocaInst *localAlloca = nullptr;

  // Register all variables and emit their initializer.
  for (auto l : n->VARS) {
    localAlloca = CreateEntryBlockAlloca(TheFunction, l);

    // Initialize all locals to "0"
    Builder.CreateStore(ConstantInt::get(Type::getInt64Ty(Context), 0),
                        localAlloca);

    // Remember this binding.
    NamedValues[l] = localAlloca;
  }

  // Return the body computation.
  codegenValue = localAlloca;
}

void CodeGenerator::visit(AST::AssignStmt *n) {
  /*  
   * Generate l-value code for the top-most expression of the LHS.
   * In doing so treat all sub-expressions as r-values.
   */
  lValueGen = true;

  n->LHS->accept(this);
  if (codegenValue == nullptr) return;
  Value *lValue = codegenValue;

  n->RHS->accept(this);
  if (codegenValue == nullptr) return;
  Value *rValue = codegenValue;

  codegenValue = Builder.CreateStore(rValue, lValue);
}

void CodeGenerator::visit(AST::BlockStmt *n) {
  Value *lastStmt = nullptr;

  for (auto const &s : n->STMTS) {
    s->accept(this);
    if (codegenValue == nullptr) return;
    lastStmt = codegenValue;
  }

  // If the block was empty return a nop
  codegenValue = (lastStmt == nullptr) ? Builder.CreateCall(nop) : lastStmt;
}

/*
 * The code generated for an WhileStmt looks like this:
 *
 *       <COND> == 0		this is called the "header" block
 *   true   /  ^  \   false
 *         v  /    \
 *      <BODY>     /
 *                /
 *               v
 *              nop        	this is called the "exit" block
 *
 * Much of the code involves setting up the different blocks, establishing
 * the insertion point, and then letting other codegen functions write
 * code at that insertion point.  A key difference is that the condition
 * is generated into a basic block since it will be branched to after the
 * body executes.
 */
void CodeGenerator::visit(AST::WhileStmt *n) {
  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  /*
   * Create blocks for the loop header, body, and exit; HeaderBB is first
   * so it is added to the function in the constructor.
   *
   * Blocks don't need to be contiguous or ordered in
   * any particular way because we will explicitly branch between them.
   * This can be optimized by later passes.
   */
  labelNum++; // create unique labels for these BBs

  BasicBlock *HeaderBB = BasicBlock::Create(
      Context, "header" + std::to_string(labelNum), TheFunction);
  BasicBlock *BodyBB =
      BasicBlock::Create(Context, "body" + std::to_string(labelNum));
  BasicBlock *ExitBB =
      BasicBlock::Create(Context, "exit" + std::to_string(labelNum));

  // Add an explicit branch from the current BB to the header
  Builder.CreateBr(HeaderBB);

  // Emit loop header
  {
    Builder.SetInsertPoint(HeaderBB);

    n->COND->accept(this);
    if (codegenValue == nullptr) return;
    Value *CondV = codegenValue;

    // Convert condition to a bool by comparing non-equal to 0.
    CondV = Builder.CreateICmpNE(CondV, ConstantInt::get(CondV->getType(), 0),
                                 "loopcond");

    Builder.CreateCondBr(CondV, BodyBB, ExitBB);
  }

  // Emit loop body
  {
    TheFunction->getBasicBlockList().push_back(BodyBB);
    Builder.SetInsertPoint(BodyBB);

    n->BODY->accept(this);
    if (codegenValue == nullptr) return;
    Value *BodyV = codegenValue;

    Builder.CreateBr(HeaderBB);
  }

  // Emit loop exit block.
  TheFunction->getBasicBlockList().push_back(ExitBB);
  Builder.SetInsertPoint(ExitBB);
  codegenValue = Builder.CreateCall(nop);
}

/*
 * The code generated for an IfStmt looks like this:
 *
 *       <COND> == 0
 *   true   /     \   false
 *         v       v
 *      <THEN>   <ELSE>  if defined, otherwise use a nop
 *          \     /
 *           v   v
 *            nop        this is called the merge basic block
 *
 * Much of the code involves setting up the different blocks, establishing
 * the insertion point, and then letting other codegen functions write
 * code at that insertion point.
 */
void CodeGenerator::visit(AST::IfStmt *n) {
  n->COND->accept(this);
  if (codegenValue == nullptr) return;
  Value *CondV = codegenValue;

  // Convert condition to a bool by comparing non-equal to 0.
  CondV = Builder.CreateICmpNE(CondV, ConstantInt::get(CondV->getType(), 0),
                               "ifcond");

  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  /*
   * Create blocks for the then and else cases.  The then block is first so
   * it is inserted in the function in the constructor.
   *
   * Blocks don't need to be contiguous or ordered in
   * any particular way because we will explicitly branch between them.
   * This can be optimized to fall through behavior by later passes.
   */
  labelNum++; // create unique labels for these BBs
  BasicBlock *ThenBB = BasicBlock::Create(
      Context, "then" + std::to_string(labelNum), TheFunction);
  BasicBlock *ElseBB =
      BasicBlock::Create(Context, "else" + std::to_string(labelNum));
  BasicBlock *MergeBB =
      BasicBlock::Create(Context, "ifmerge" + std::to_string(labelNum));

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then block.
  {
    Builder.SetInsertPoint(ThenBB);

    n->THEN->accept(this);
    if (codegenValue == nullptr) return;
    Value *ThenV = codegenValue;

    Builder.CreateBr(MergeBB);
  }

  // Emit else block.
  {
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    // if there is no ELSE then exist emit a "nop"
    Value *ElseV = nullptr;
    if (n->ELSE != nullptr) {
      n->ELSE->accept(this);
      if (codegenValue == nullptr) return;
      ElseV = codegenValue;
    } else {
      Builder.CreateCall(nop);
    }

    Builder.CreateBr(MergeBB);
  }

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  codegenValue = Builder.CreateCall(nop);
}

void CodeGenerator::visit(AST::OutputStmt *n) {
  if (outputIntrinsic == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(Context));
    auto *FT = FunctionType::get(Type::getInt64Ty(Context), oneInt, false);
    outputIntrinsic =
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                               "_tip_output", CurrentModule.get());
  }

  n->ARG->accept(this);
  if (codegenValue == nullptr) return;
  Value *ArgVal = codegenValue;

  std::vector<Value *> ArgsV(1, ArgVal);

  codegenValue = Builder.CreateCall(outputIntrinsic, ArgsV);
}

void CodeGenerator::visit(AST::ErrorStmt *n) {
  if (errorIntrinsic == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(Context));
    auto *FT = FunctionType::get(Type::getInt64Ty(Context), oneInt, false);
    errorIntrinsic = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                            "_tip_error", CurrentModule.get());
  }

  n->ARG->accept(this);
  if (codegenValue == nullptr) return;
  Value *ArgVal = codegenValue;

  std::vector<Value *> ArgsV(1, ArgVal);

  codegenValue = Builder.CreateCall(errorIntrinsic, ArgsV);
}

void CodeGenerator::visit(AST::ReturnStmt *n) {
  n->ARG->accept(this);
  if (codegenValue == nullptr) return;
  Value *ArgVal = codegenValue;
  codegenValue = Builder.CreateRet(ArgVal);
}

/******************* utility routines ******************/

/*
 * Create LLVM Function in Module associated with current program.
 * This function declares the function, but it does not generate code.
 * This is a key element of the shallow pass that builds the function
 * dispatch table.
 */
llvm::Function *CodeGenerator::getFunction(std::string Name) {
  // Lookup the symbol to access the formal parameter list
  auto idx_formals = FunctionDecls[Name];

  /*
   * Main is handled specially.  It is declared as "_tip_main" with
   * no arguments - any arguments are converted to locals with special
   * initializaton in Function visit method.
   */
  if (Name == "main") {
    if (auto *M = CurrentModule->getFunction("_tip_main")) {
      return M;
    }

    // initialize the number of TIP program args for initializing globals
    numTIPArgs = idx_formals.second.size();

    // Declare "_tip_main()"
    auto *M = llvm::Function::Create(
        FunctionType::get(Type::getInt64Ty(Context), false),
        llvm::Function::ExternalLinkage, "_tip_" + Name, CurrentModule.get());
    return M;
  } else {
    // check if function is in the current module
    if (auto *F = CurrentModule->getFunction(Name)) {
      return F;
    }

    std::vector<Type *> FormalTypes(idx_formals.second.size(),
                                    Type::getInt64Ty(Context));

    // Use type factory to create function from formal type to int
    auto *FT =
        FunctionType::get(Type::getInt64Ty(Context), FormalTypes, false);

    auto *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name,
                                     CurrentModule.get());

    // assign names to args for readability of generated code
    unsigned i = 0;
    for (auto &param : F->args()) {
      param.setName(idx_formals.second[i++]);
    }

    return F;
  }
}

/*
 * Create an alloca instruction in the entry block of the function.
 * This is used for mutable variables, including arguments to functions.
 */
AllocaInst *CodeGenerator::CreateEntryBlockAlloca(
                llvm::Function *TheFunction, const std::string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt64Ty(Context), 0, VarName);
}

Value *CodeGenerator::LogError(std::string s) {
  fprintf(stderr, "Error: %s\n", s.c_str());
  return nullptr;
}
