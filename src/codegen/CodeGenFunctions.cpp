#include "AST.h"
#include "SemanticAnalysis.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/TypeSize.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include "llvm-c/TargetMachine.h"

using namespace llvm;

/*
 * Code Generation Routines from TIP Tree Representation
 *
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

namespace {
/*
 * These structures records lots of information that spans the codegen()
 * routines.  For example, the current insertion, entry, return blocks
 * in the current function.
 */
LLVMContext TheContext;
IRBuilder<> Builder(TheContext);

// Functions are represented with indices into a table; this permits them to be passed as values
std::map<std::string, int> functionIndex;

/* TBD UPDATE TYPE CHECKER
 * We currently rely on the fact that all function parameters are integer
 * type, so we don't need to indicate the type explicitly.  Ultimately, we will
 * want to use the type information by dropping this data structure and using
 * type infor in the code generation routine.
 */
std::map<std::string, std::vector<std::string>> functionFormalNames;

/*
 * This structure stores the mapping from names in a function scope
 * to their LLVM values.  The structure is built when entering a
 * scope and cleared when exiting a scope.
 */
// TBD SYMBOL TABLE this could really be a mapping from DeclNode to AllocaInst
std::map<std::string, AllocaInst *> NamedValues;


/**
 * The UberRecord is a the type of all records
 *  It has a field for every named field in the program
 */
llvm::StructType * uberRecordType;

/**
 * The type for pointers to UberRecordType
 */
llvm::PointerType * ptrToUberRecordType;

//Maps field names to their index in the UberRecor
std::map<std::basic_string<char>, int> fieldIndex;

//Vector of fields in an uber record
std::vector<std::basic_string<char>> fieldVector;

// Permits getFunction to access the current module being compiled
std::unique_ptr<Module> CurrentModule;

/*
 * We use calls to llvm intrinsics for several purposes.  To construct a "nop",
 * using an LLVM internal intrinsic, to perform TIP specific IO, and
 * to allocate heap memory.
 */
llvm::Function *nop = nullptr;
llvm::Function *inputIntrinsic = nullptr;
llvm::Function *outputIntrinsic = nullptr;
llvm::Function *errorIntrinsic = nullptr;
llvm::Function *callocFun = nullptr;


// A counter to create unique labels
int labelNum = 0;

// Indicate whether the expression code gen is for an L-value
bool lValueGen = false;

/*
 * The global function dispatch table is created in a shallow pass over
 * the function signatures, stored here, and then referenced in generating
 * function applications.
 */
GlobalVariable *tipFTable = nullptr;

// The number of TIP program parameters
int numTIPArgs = 0;

/*
 * The global argument count and array are used to communicate command
 * line inputs to the TIP main function.
 */
GlobalVariable *tipNumInputs = nullptr;
GlobalVariable *tipInputArray = nullptr;

/*
 * Create LLVM Function in Module associated with current program.
 * This function declares the function, but it does not generate code.
 * This is a key element of the shallow pass that builds the function
 * dispatch table.
 */
llvm::Function *getFunction(std::string Name) {
  // Lookup the symbol to access the formal parameter list
  // TBD TYPE CHECKER - update this
  auto formals = functionFormalNames[Name];

  /*
   * Main is handled specially.  It is declared as "_tip_main" with
   * no arguments - any arguments are converted to locals with special
   * initializaton in Function::codegen().
   */
  if (Name == "main") {
    if (auto *M = CurrentModule->getFunction("_tip_main")) {
      return M;
    }

    // initialize the number of TIP program args for initializing globals
    numTIPArgs = formals.size();

    // Declare "_tip_main()"
    auto *M = llvm::Function::Create(
        FunctionType::get(Type::getInt64Ty(TheContext), false),
        llvm::Function::ExternalLinkage, "_tip_" + Name, CurrentModule.get());
    return M;
  } else {
    // check if function is in the current module
    if (auto *F = CurrentModule->getFunction(Name)) {
      return F;
    }

    // function not found, so create it

    std::vector<Type *> FormalTypes(formals.size(), Type::getInt64Ty(TheContext));

    // Use type factory to create function from formal type to int
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), FormalTypes, false);

    auto *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name,
                                     CurrentModule.get());

    // assign names to args for readability of generated code
    unsigned i = 0;
    for (auto &param : F->args()) {
      param.setName(formals[i++]);
    }

    return F;
  }
}

/*
 * Create an alloca instruction in the entry block of the function.
 * This is used for mutable variables, including arguments to functions.
 */
AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, const std::string &VarName) {
  IRBuilder<> tmp(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return tmp.CreateAlloca(Type::getInt64Ty(TheContext), 0, VarName);
}

Value *LogError(std::string s) {
  fprintf(stderr, "Code Generation Error: %s\n", s.c_str());
  return nullptr;
}

} // end anonymous namespace for code generator data and functions

/********************* codegen() routines ************************/

std::unique_ptr<llvm::Module> ASTProgram::codegen(SemanticAnalysis* analysis,
                                                  std::string programName) {
  // Create module to hold generated code
  auto TheModule = std::make_unique<Module>(programName, TheContext);

  // Set the default target triple for this platform
  TheModule->setTargetTriple(LLVMGetDefaultTargetTriple());

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
    for (auto const &fn : getFunctions()) {
      functionIndex[fn->getName()] = funIndex++;

      // TBD TYPE CHECK - drop this once we have type information
      auto formals = fn->getFormals();
      std::vector<std::string> names;
      std::transform(formals.begin(), formals.end(), 
                     std::back_inserter(names), [](auto& d){return d->getName();});
      functionFormalNames[fn->getName()] = names;
    }

    /*
     * Create the llvm functions.
     * Store as a vector of constants, which works because Function
     * is a subtype of Constant, to workaround the inability of the
     * compiler to find a conversion from Function to Constant
     * below in creating the ftableInit.
     */
    std::vector<llvm::Constant *> programFunctions;
    for (auto const &fn : getFunctions()) {
      programFunctions.push_back(getFunction(fn->getName()));
    }

    /*
     * Create a generic function pointer type "() -> Int64" that is
     * used to declare the global function dispatch table and to
     * bitcast the declared functions prior to inserting them.
     */
    auto *genFunPtrType = PointerType::get(
        FunctionType::get(Type::getInt64Ty(TheContext), None, false), 0);

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
     * we never visit it during the codegen() traversals - since
     * the function doesn't exist in the TIP program.
     */
    auto fidx = functionIndex.find("main");
    if (fidx == functionIndex.end()) {
      auto *M = llvm::Function::Create(
          FunctionType::get(Type::getInt64Ty(TheContext), false),
          llvm::Function::ExternalLinkage, "_tip_main", CurrentModule.get());
      BasicBlock *BB = BasicBlock::Create(TheContext, "entry", M);
      Builder.SetInsertPoint(BB);

      auto *undef = llvm::Function::Create(
          FunctionType::get(Type::getVoidTy(TheContext), false),
          llvm::Function::ExternalLinkage, "_tip_main_undefined",
          CurrentModule.get());
      Builder.CreateCall(undef);
      Builder.CreateRet(ConstantInt::get(Type::getInt64Ty(TheContext), 0));
    }

    // create global _tip_num_inputs with init of numTIPArgs
    tipNumInputs = new GlobalVariable(
        *CurrentModule, Type::getInt64Ty(TheContext), true,
        llvm::GlobalValue::ExternalLinkage,
        ConstantInt::get(Type::getInt64Ty(TheContext), numTIPArgs),
        "_tip_num_inputs");

    // create global _tip_input_array with up to numTIPArgs of Int64
    auto *inputArrayType =
        ArrayType::get(Type::getInt64Ty(TheContext), numTIPArgs);
    std::vector<Constant *> zeros(
        numTIPArgs, ConstantInt::get(Type::getInt64Ty(TheContext), 0));
    tipInputArray = new GlobalVariable(
        *CurrentModule, inputArrayType, false, llvm::GlobalValue::CommonLinkage,
        ConstantArray::get(inputArrayType, zeros), "_tip_input_array");
  }

  // declare the calloc function
  // the calloc function takes in two ints: the number of items and the size of the items
  std::vector<Type *> twoInt(2, Type::getInt64Ty(TheContext));
  auto *FT = FunctionType::get(Type::getInt8PtrTy(TheContext), twoInt, false);
  callocFun = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                     "calloc", CurrentModule.get());
  callocFun->addFnAttr(llvm::Attribute::NoUnwind);
  callocFun->addAttribute(0, llvm::Attribute::NoAlias);

  std::vector<Type *> member_values;
  int index = 0;
  for(auto field : analysis->getSymbolTable()->getFields()){
      member_values.push_back(IntegerType::getInt64Ty((TheContext)));
      fieldVector.push_back(field);
      fieldIndex[field] = index;
      index++;
  }


  uberRecordType = StructType::create(TheContext, member_values, "uberRecord");
  ptrToUberRecordType = PointerType::get(uberRecordType, 0);


  // Code is generated into the module by the other routines
  for (auto const &fn : getFunctions()) {
    fn->codegen();
  }

  TheModule = std::move(CurrentModule);

  verifyModule(*TheModule);

  return TheModule;
}

llvm::Value* ASTFunction::codegen() {
  bool success = true;

  llvm::Function *TheFunction = getFunction(getName());
  if (TheFunction == nullptr) {
    return nullptr;
  }

  // create basic block to hold body of function definition
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);

  // keep scope separate from prior definitions
  NamedValues.clear();

  /*
   * Add arguments to the symbol table
   *   - for main function, we initialize allocas with array loads
   *   - for other functions, we initialize allocas with the arg values
   */
  if (getName() == "main") {
    int argIdx = 0;
    // Note that the args are not in the LLVM function decl, so we use the AST formals
    // TBD TYPE CHECK update
    for (auto &argName : functionFormalNames[getName()]) {
      // Create an alloca for this argument and store its value
      AllocaInst *argAlloc = CreateEntryBlockAlloca(TheFunction, argName);

      // Emit the GEP instruction to index into input array
      std::vector<Value *> indices;
      indices.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 0));
      indices.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), argIdx));
      auto *gep = Builder.CreateInBoundsGEP(tipInputArray, indices, "inputidx");

      // Load the value and store it into the arg's alloca
      auto *inVal = Builder.CreateLoad(gep, "tipinput" + std::to_string(argIdx++));
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

  // add local declarations to the symbol table
  for (auto const &decl : getDeclarations()) {
    if (decl->codegen() == nullptr) {
      success = false;
    }
  }

  for (auto &stmt : getStmts()) {
    if (stmt->codegen() == nullptr) {
      success = false;
    }
  }

  if (success) {
    // internal LLVM helper function to detect errors in function defs
    verifyFunction(*TheFunction);
    return TheFunction;
  }

  // error in generating body, so remove partial function definition
  TheFunction->eraseFromParent();
  return nullptr;
}

llvm::Value* ASTNumberExpr::codegen() {
  return ConstantInt::get(Type::getInt64Ty(TheContext), getValue());
}

llvm::Value* ASTBinaryExpr::codegen() {
  Value *L = getLeft()->codegen();
  Value *R = getRight()->codegen();
  if (L == nullptr || R == nullptr) {
    return nullptr;
  }

  if (getOp() == "+") {
    return Builder.CreateAdd(L, R, "addtmp");
  } else if (getOp() == "-") {
    return Builder.CreateSub(L, R, "subtmp");
  } else if (getOp() == "*") {
    return Builder.CreateMul(L, R, "multmp");
  } else if (getOp() == "/") {
    return Builder.CreateSDiv(L, R, "divtmp");
  } else if (getOp() == ">") {
    return Builder.CreateICmpSGT(L, R, "gttmp");
  } else if (getOp() == "==") {
    return Builder.CreateICmpEQ(L, R, "eqtmp");
  } else if (getOp() == "!=") {
    return Builder.CreateICmpNE(L, R, "netmp");
  } else {
    return LogError("Invalid binary operator: " + OP);
  }
}

/*
 * First lookup the variable in the symbol table for names and
 * if that fails, then look in the symbol table for functions.
 *
 * This relies on the fact that TIP programs do not permit duplicate names.
 */
llvm::Value* ASTVariableExpr::codegen() {
  auto nv = NamedValues.find(getName());
  if (nv != NamedValues.end()) {
    if (lValueGen) {
      return NamedValues[nv->first];
    } else {
      return Builder.CreateLoad(nv->second, getName().c_str());
    }
  }

  auto fidx = functionIndex.find(getName());
  if (fidx == functionIndex.end()) {
    return LogError("Unknown variable name: " + getName());
  }

  return ConstantInt::get(Type::getInt64Ty(TheContext), fidx->second);
}

llvm::Value* ASTInputExpr::codegen() {
  if (inputIntrinsic == nullptr) {
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), false);
    inputIntrinsic = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                            "_tip_input", CurrentModule.get());
  }
  return Builder.CreateCall(inputIntrinsic);
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
llvm::Value* ASTFunAppExpr::codegen() {
  /*
   * Evaluate the function expression - it will resolve to an integer value
   * whether it is a function literal or an expression.
   */
  auto *funVal = getFunction()->codegen();
  if (funVal == nullptr) {
    return nullptr;
  }

  /*
   * Emit the GEP instruction to compute the address of LLVM function
   * pointer to be called.
   */
  std::vector<Value *> indices;
  indices.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 0));
  indices.push_back(funVal);
  auto *gep = Builder.CreateInBoundsGEP(tipFTable, indices, "ftableidx");

  // Load the function pointer
  auto *genericFunPtr = Builder.CreateLoad(gep, "genfptr");

  /*
   * Compute the specific function pointer type based on the actual parameter
   * list.
   *
   * TBD: Currently the type is Int64^N -> Int64, * where N is the length of the list
   *
   * Once type information is available we will need to iterate the actuals
   * and construct the per actual vector of types.
   */
  std::vector<Type *> actualTypes(getActuals().size(), Type::getInt64Ty(TheContext));
  auto *funPtrType = PointerType::get(
      FunctionType::get(Type::getInt64Ty(TheContext), actualTypes, false), 0);

  // Bitcast the function pointer to the call-site determined function type
  auto *castFunPtr =
      Builder.CreatePointerCast(genericFunPtr, funPtrType, "castfptr");

  // Compute the actual parameters
  std::vector<Value *> argsV;
  for (auto const &arg : getActuals()) {
    Value *argVal = arg->codegen();
    if (argVal == nullptr) {
      return nullptr;
    }
    argsV.push_back(argVal);
  }

  return Builder.CreateCall(castFunPtr, argsV, "calltmp");
}

llvm::Value* ASTAllocExpr::codegen() {
  Value *argVal = getInitializer()->codegen();
  if (argVal == nullptr) {
    return nullptr;
  }

  // TBD TYPE CHECKING - this needs upating
  // Since we do not support records all allocs are for 8 bytes, i.e., int64_t
  std::vector<Value *> twoArg;
  twoArg.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 1));
  twoArg.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 8));
  auto *allocInst = Builder.CreateCall(callocFun, twoArg, "allocPtr");
  auto *castPtr = Builder.CreatePointerCast(
      allocInst, Type::getInt64PtrTy(TheContext), "castPtr");
  // Initialize with argument
  auto *initializingStore = Builder.CreateStore(argVal, castPtr);

  return Builder.CreatePtrToInt(castPtr, Type::getInt64Ty(TheContext),
                                "allocIntVal");
}

llvm::Value* ASTNullExpr::codegen() {
  auto *nullPtr = ConstantPointerNull::get(Type::getInt64PtrTy(TheContext));
  return Builder.CreatePtrToInt(nullPtr, Type::getInt64Ty(TheContext),
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
llvm::Value* ASTRefExpr::codegen() {
  auto *v = dynamic_cast<ASTVariableExpr*>(getVar());
  Value *argVal = NamedValues[v->getName()];
  if (argVal == nullptr) {
    return LogError("Unknown variable name: " + v->getName());
  }

  return Builder.CreatePtrToInt(argVal, Type::getInt64Ty(TheContext),
                                "intPtrVal");
}

/* '*' dereference expression
 *
 * The argument is assumed to be a reference expression, but
 * our code generation strategy stores everything as an integer.
 * Consequently, we convert the value with "inttoptr" before loading
 * the value at the pointed-to memory location.
 */
llvm::Value* ASTDeRefExpr::codegen() {
  bool isLValue = lValueGen;

  if (isLValue) {
    // This flag is reset here so that sub-expressions are treated as r-values
    lValueGen = false;
  }
 
  Value *argVal = getPtr()->codegen();
  if (argVal == nullptr) {
    return nullptr;
  }

  // compute the address
  Value *address = Builder.CreateIntToPtr(argVal, Type::getInt64PtrTy(TheContext), "ptrIntVal");

  if (isLValue) {
    // For an l-value, return the address
    return address;
  } else {
    // For an r-value, return the value at the address
    return Builder.CreateLoad(address, "valueAt");
  }
}

/* {field1 : val1, ..., fieldN, valN} record expression
 *
 * Builds an instance of the UberRecord using the declared fields
 */
llvm::Value* ASTRecordExpr::codegen() {
  //Allocate the a pointer to an uber record
  auto *allocaRecord = Builder.CreateAlloca(ptrToUberRecordType);

  // Use Builder to create the calloc call using pre-defined callocFun
  auto sizeOfUberRecord = CurrentModule->getDataLayout().getStructLayout(uberRecordType)->getSizeInBytes();
  std::vector<Value *> callocArgs;
  callocArgs.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 1));
  callocArgs.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), sizeOfUberRecord));
  auto *calloc = Builder.CreateCall(callocFun, callocArgs, "callocedPtr");

  //Bitcast the calloc call to theStruct Type
  auto *recordPtr = Builder.CreatePointerCast(calloc, ptrToUberRecordType, "recordCalloc");

  //Store the ptr to the record in the record alloc
  Builder.CreateStore(recordPtr, allocaRecord);

  //Load allocaRecord
  auto loadInst = Builder.CreateLoad(ptrToUberRecordType,allocaRecord);

  //For each field, generate GEP for location of field in the uberRecord
  //Generate the code for the field and store it in the GEP
  for(auto const &field : getFields()){
      auto *gep = Builder.CreateStructGEP(uberRecordType, loadInst, fieldIndex[field->getField()], field->getField());
      auto value = field->codegen();
      Builder.CreateStore(value, gep);
  }

  //Return int64 pointer to the record
  return Builder.CreatePtrToInt(recordPtr, Type::getInt64Ty(TheContext), "recordPtr");
}

/* field : val field expression
 *
 * Expression for generating the code for the value of a field
 */
llvm::Value* ASTFieldExpr::codegen() {
  return this->getInitializer()->codegen();
}

/* record.field Access Expression
 *
 * On the LHS of the expression, returns the location
 * of the field being accessed
 * On the RHS of the expression, returns the value
 * of the field being accessed
 */
llvm::Value* ASTAccessExpr::codegen() {
    bool isLValue = lValueGen;

    if (isLValue) {
        // This flag is reset here so that sub-expressions are treated as r-values
        lValueGen = false;
    }

    //Get current field and check if it exists
    auto currField = this->getField();
    if(fieldIndex.count(currField) == 0){
        return LogError("This field doesn't exist");
    }

  //Generate record instruction address
  Value *recordVal = this->getRecord()->codegen();
  Value *recordAddress = Builder.CreateIntToPtr(recordVal, ptrToUberRecordType);

  //Generate the field index
  auto index = fieldIndex[currField];

  //Generate the location of the field
  auto *gep = Builder.CreateStructGEP(uberRecordType, recordAddress, index, currField);

  //If LHS, return location of field
  if(isLValue){
      return gep;
  }

  //Load value at GEP and return it
  auto fieldLoad = Builder.CreateLoad(IntegerType::getInt64Ty(TheContext), gep);
  return Builder.CreatePtrToInt(fieldLoad, Type::getInt64Ty(TheContext), "fieldAccess");
}

llvm::Value* ASTDeclNode::codegen() {
  return LogError("Declarations do not emit code");
}

llvm::Value* ASTDeclStmt::codegen() {
  // The LLVM builder records the function we are currently generating
  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  AllocaInst *localAlloca = nullptr;

  // Register all variables and emit their initializer.
  for (auto l : getVars()) {
    localAlloca = CreateEntryBlockAlloca(TheFunction, l->getName());

    // Initialize all locals to "0"
    Builder.CreateStore(ConstantInt::get(Type::getInt64Ty(TheContext), 0),
                        localAlloca);

    // Remember this binding.
    NamedValues[l->getName()] = localAlloca;
  }

  // Return the body computation.
  return localAlloca;
}

llvm::Value* ASTAssignStmt::codegen() {
  // trigger code generation for l-value expressions
  lValueGen = true;
  Value *lValue = getLHS()->codegen();
  lValueGen = false;

  if (lValue == nullptr) {
    return nullptr;
  }

  Value *rValue = getRHS()->codegen();
  if (rValue == nullptr) {
    return nullptr;
  }

  return Builder.CreateStore(rValue, lValue);
}



llvm::Value* ASTBlockStmt::codegen() {
  Value *lastStmt = nullptr;

  for (auto const &s : getStmts()) {
    lastStmt = s->codegen();
  }

  // If the block was empty return a nop
  return (lastStmt == nullptr) ? Builder.CreateCall(nop) : lastStmt;
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
llvm::Value* ASTWhileStmt::codegen() {
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
      TheContext, "header" + std::to_string(labelNum), TheFunction);
  BasicBlock *BodyBB =
      BasicBlock::Create(TheContext, "body" + std::to_string(labelNum));
  BasicBlock *ExitBB =
      BasicBlock::Create(TheContext, "exit" + std::to_string(labelNum));

  // Add an explicit branch from the current BB to the header
  Builder.CreateBr(HeaderBB);

  // Emit loop header
  {
    Builder.SetInsertPoint(HeaderBB);

    Value *CondV = getCondition()->codegen();
    if (CondV == nullptr) {
      return nullptr;
    }

    // Convert condition to a bool by comparing non-equal to 0.
    CondV = Builder.CreateICmpNE(CondV, ConstantInt::get(CondV->getType(), 0),
                                 "loopcond");

    Builder.CreateCondBr(CondV, BodyBB, ExitBB);
  }

  // Emit loop body
  {
    TheFunction->getBasicBlockList().push_back(BodyBB);
    Builder.SetInsertPoint(BodyBB);

    Value *BodyV = getBody()->codegen();
    if (BodyV == nullptr) {
      return nullptr;
    }

    Builder.CreateBr(HeaderBB);
  }

  // Emit loop exit block.
  TheFunction->getBasicBlockList().push_back(ExitBB);
  Builder.SetInsertPoint(ExitBB);
  return Builder.CreateCall(nop);
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
llvm::Value* ASTIfStmt::codegen() {
  Value *CondV = getCondition()->codegen();
  if (CondV == nullptr) {
    return nullptr;
  }

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
      TheContext, "then" + std::to_string(labelNum), TheFunction);
  BasicBlock *ElseBB =
      BasicBlock::Create(TheContext, "else" + std::to_string(labelNum));
  BasicBlock *MergeBB =
      BasicBlock::Create(TheContext, "ifmerge" + std::to_string(labelNum));

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then block.
  {
    Builder.SetInsertPoint(ThenBB);

    Value *ThenV = getThen()->codegen();
    if (ThenV == nullptr) {
      return nullptr;
    }

    Builder.CreateBr(MergeBB);
  }

  // Emit else block.
  {
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    // if there is no ELSE then exist emit a "nop"
    Value *ElseV = nullptr;
    if (getElse() != nullptr) {
      ElseV = getElse()->codegen();
      if (ElseV == nullptr) {
        return nullptr;
      }
    } else {
      Builder.CreateCall(nop);
    }

    Builder.CreateBr(MergeBB);
  }

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  return Builder.CreateCall(nop);
}

llvm::Value* ASTOutputStmt::codegen() {
  if (outputIntrinsic == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(TheContext));
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), oneInt, false);
    outputIntrinsic =
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                               "_tip_output", CurrentModule.get());
  }

  Value *argVal = getArg()->codegen();
  if (argVal == nullptr) {
    return nullptr;
  }

  std::vector<Value *> ArgsV(1, argVal);

  return Builder.CreateCall(outputIntrinsic, ArgsV);
}

llvm::Value* ASTErrorStmt::codegen() {
  if (errorIntrinsic == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(TheContext));
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), oneInt, false);
    errorIntrinsic = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                            "_tip_error", CurrentModule.get());
  }

  Value *argVal = getArg()->codegen();
  if (argVal == nullptr) {
    return nullptr;
  }

  std::vector<Value *> ArgsV(1, argVal);

  return Builder.CreateCall(errorIntrinsic, ArgsV);
}

llvm::Value* ASTReturnStmt::codegen() {
  Value *argVal = getArg()->codegen();
  return Builder.CreateRet(argVal);
}
