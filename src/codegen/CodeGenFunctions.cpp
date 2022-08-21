
#include <ASTDeclNode.h>

#include "AST.h"
#include "SemanticAnalysis.h"
#include "InternalError.h"

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
#include "llvm/Support/Host.h"
#include "llvm/Support/TypeSize.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "loguru.hpp"

using namespace llvm;

/*
 * Code Generation Routines from TIP Tree Representation
 *
 * These code generation routines are based on the LLVM Kaleidoscope
 * Tutorial for C++ Chapters 3 and 7
 *     https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html
 *     https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl07.html
 * We thank the authors for making these example available.
 *
 * Unlike the tutorial, these routines rely on following LLVM passes
 * to transform the code to SSA form, via the PromoteMemToReg pass,
 * and especially the CFGSimplification pass, which removes nops and
 * dead blocks.
 *
 * The approach taken in these codegen routines is to rely on the fact
 * that the program is type correct, which is checked in a previous pass.
 * This allows the code generator to infer the type of values accessed
 * in memory based on the nature of the expression, e.g., if we are generating
 * code for "*p" then "p" must be a pointer.  This approach is facilitated
 * by choosing a uniform representation of all types in memory, Int64, and
 * then as needed inserting type conversions into the generated code to convert
 * them to the appropriate type for an operator.
 *
 * This results in some suboptimal code, but we rely on the powerful LLVM
 * optimization passes to clean most of it up.  
 */

namespace {
/*
 * These structures records lots of information that spans the codegen()
 * routines.  For example, the current insertion, entry, return blocks
 * in the current function.
 */
LLVMContext TheContext;
IRBuilder<> Builder(TheContext);

/* 
 * Functions are represented with indices into a table. 
 * This permits function values to be passed, i.e, as Int64 indices. 
 */
std::map<std::string, int> functionIndex;

std::map<std::string, std::vector<std::string>> functionFormalNames;

/*
 * This structure stores the mapping from names in a function scope
 * to their LLVM values.  The structure is built when entering a
 * scope and cleared when exiting a scope.
 */
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

// Maps field names to their index in the UberRecor
std::map<std::basic_string<char>, int> fieldIndex;

// Vector of fields in an uber record
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

// Indicate whether the expression code gen is for an alloc'd value
bool allocFlag = false;

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
 * Some constants are used repeatedly in code generation.  We define them
 * hear to eliminate redundancy.
 */
Constant *zeroV = ConstantInt::get(Type::getInt64Ty(TheContext), 0);
Constant *oneV = ConstantInt::get(Type::getInt64Ty(TheContext), 1);

/*
 * Create LLVM Function in Module associated with current program.
 * This function declares the function, but it does not generate code.
 * This is a key element of the shallow pass that builds the function
 * dispatch table.
 */
llvm::Function *getFunction(std::string Name) {
  // Lookup the symbol to access the formal parameter list
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

    auto *F = llvm::Function::Create(FT, llvm::Function::InternalLinkage, Name,
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

} // end anonymous namespace for code generator data and functions

/********************* codegen() routines ************************/

std::unique_ptr<llvm::Module> ASTProgram::codegen(SemanticAnalysis* analysis,
                                                  std::string programName) {
  LOG_S(1) << "Generating code for program " << programName;

  // Create module to hold generated code
  auto TheModule = std::make_unique<Module>(programName, TheContext);

  // Set the default target triple for this platform
  llvm:Triple targetTriple(llvm::sys::getProcessTriple());
  TheModule->setTargetTriple(targetTriple.str());

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
      Builder.CreateRet(zeroV);
    }

    // create global _tip_num_inputs with init of numTIPArgs
    tipNumInputs = new GlobalVariable(
        *CurrentModule, Type::getInt64Ty(TheContext), true,
        llvm::GlobalValue::ExternalLinkage,
        ConstantInt::get(Type::getInt64Ty(TheContext), numTIPArgs),
        "_tip_num_inputs");

    // create global _tip_input_array with up to numTIPArgs of Int64
    auto *inputArrayType = ArrayType::get(Type::getInt64Ty(TheContext), numTIPArgs);
    std::vector<Constant *> zeros(numTIPArgs, zeroV);
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

  callocFun->setAttributes(callocFun->getAttributes().addAttributeAtIndex(callocFun->getContext(), 0, llvm::Attribute::NoAlias));

  /* We create a single unified record structure that is capable of representing
   * all records in a TIP program.  While wasteful of memory, this approach is 
   * compatible with the limited type checking provided for records in TIP.
   *
   * We refer to this single unified record structure as the "uber record"
   */
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
  LOG_S(1) << "Generating code for " << *this;

  llvm::Function *TheFunction = getFunction(getName());
  if (TheFunction == nullptr) {
    throw InternalError("failed to declare the function" + getName()); // LCOV_EXCL_LINE
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
    for (auto &argName : functionFormalNames[getName()]) {
      // Create an alloca for this argument and store its value
      AllocaInst *argAlloc = CreateEntryBlockAlloca(TheFunction, argName);

      // Emit the GEP instruction to index into input array
      std::vector<Value *> indices;
      indices.push_back(zeroV); 
      indices.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), argIdx));
      auto *gep = Builder.CreateInBoundsGEP(tipInputArray->getValueType(), tipInputArray, indices, "inputidx");

      // Load the value and store it into the arg's alloca
      auto *inVal = Builder.CreateLoad(gep->getType()->getPointerElementType(), gep, "tipinput" + std::to_string(argIdx++));
      Builder.CreateStore(inVal, argAlloc);

      // Record name binding to alloca
      NamedValues[argName] = argAlloc;
    }
  } else {
    for (auto &arg : TheFunction->args()) {
      // Create an alloca for this argument and store its value
      AllocaInst *argAlloc = CreateEntryBlockAlloca(TheFunction, arg.getName().str());
      Builder.CreateStore(&arg, argAlloc);

      // Record name binding to alloca
      NamedValues[arg.getName().str()] = argAlloc;
    }
  }

  // add local declarations to the symbol table
  for (auto const &decl : getDeclarations()) {
    if (decl->codegen() == nullptr) {
      TheFunction->eraseFromParent(); // LCOV_EXCL_LINE
      throw InternalError("failed to generate bitcode for the function declarations"); // LCOV_EXCL_LINE
    }
  }

  for (auto &stmt : getStmts()) {
    if (stmt->codegen() == nullptr) {
      TheFunction->eraseFromParent(); // LCOV_EXCL_LINE
      throw InternalError("failed to generate bitcode for the function statement"); // LCOV_EXCL_LINE
    }
  }

  verifyFunction(*TheFunction);
  return TheFunction;
}  // LCOV_EXCL_LINE

llvm::Value* ASTNumberExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  return ConstantInt::get(Type::getInt64Ty(TheContext), getValue());
} // LCOV_EXCL_LINE

llvm::Value* ASTBinaryExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  Value *L = getLeft()->codegen();
  Value *R = getRight()->codegen();
  if (L == nullptr || R == nullptr) {
    throw InternalError("null binary operand");
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
    throw InternalError("Invalid binary operator: " + OP);
  }
}

/*
 * First lookup the variable in the symbol table for names and
 * if that fails, then look in the symbol table for functions.
 *
 * This relies on the fact that TIP programs have been checked to
 * ensure that names obey the scope rules.
 */
llvm::Value* ASTVariableExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  auto nv = NamedValues.find(getName());
  if (nv != NamedValues.end()) {
    if (lValueGen) {
      return NamedValues[nv->first];
    } else {
      return Builder.CreateLoad(nv->second->getAllocatedType(), nv->second, getName().c_str());
    }
  }

  auto fidx = functionIndex.find(getName());
  if (fidx == functionIndex.end()) {
    throw InternalError("Unknown variable name: " + getName());
  }

  return ConstantInt::get(Type::getInt64Ty(TheContext), fidx->second);
}

llvm::Value* ASTInputExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  if (inputIntrinsic == nullptr) {
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), false);
    inputIntrinsic = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                            "_tip_input", CurrentModule.get());
  }
  return Builder.CreateCall(inputIntrinsic);
} // LCOV_EXCL_LINE

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
  LOG_S(1) << "Generating code for " << *this;

  /*
   * Evaluate the function expression - it will resolve to an integer value
   * whether it is a function literal or an expression.
   */
  auto *funVal = getFunction()->codegen();
  if (funVal == nullptr) {
    throw InternalError("failed to generate bitcode for the function");
  }

  /*
   * Emit the GEP instruction to compute the address of LLVM function
   * pointer to be called.
   */
  std::vector<Value *> indices;
  indices.push_back(zeroV); 
  indices.push_back(funVal);

  auto *gep = Builder.CreateInBoundsGEP(tipFTable->getValueType(), tipFTable, indices, "ftableidx");

  // Load the function pointer
  auto *genericFunPtr = Builder.CreateLoad(gep->getType()->getPointerElementType(), gep, "genfptr");

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
  auto *funType = FunctionType::get(Type::getInt64Ty(TheContext), actualTypes, false);
  auto *funPtrType = PointerType::get(funType, 0);


  // Bitcast the function pointer to the call-site determined function type
  auto *castFunPtr =
      Builder.CreatePointerCast(genericFunPtr, funPtrType, "castfptr");

  // Compute the actual parameters
  std::vector<Value *> argsV;
  for (auto const &arg : getActuals()) {
    Value *argVal = arg->codegen();
    if (argVal == nullptr) {
      throw InternalError("failed to generate bitcode for the argument"); // LCOV_EXCL_LINE
    }
    argsV.push_back(argVal);
  }

  return Builder.CreateCall(funType, castFunPtr, argsV, "calltmp");
}

/* 'alloc' Allocate expression
 * Generates a pointer to the allocs arguments (ints, records, ...)
 */
llvm::Value* ASTAllocExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  allocFlag = true;
  Value *argVal = getInitializer()->codegen();
  allocFlag = false;
  if (argVal == nullptr) {
    throw InternalError("failed to generate bitcode for the initializer of the alloc expression");
  }
  
  //Allocate an int pointer with calloc
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
 * The argument must be capable of generating an l-value.
 * This is checked in the weeding pass.  
 *
 */
llvm::Value* ASTRefExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  lValueGen = true;
  Value *lValue = getVar()->codegen();
  lValueGen = false;

  if (lValue == nullptr) {
    throw InternalError("could not generate l-value for address of");
  }

  return Builder.CreatePtrToInt(lValue, Type::getInt64Ty(TheContext), "addrOfPtr");
}  // LCOV_EXCL_LINE

/* '*' dereference expression
 *
 * The argument is assumed to be a reference expression, but
 * our code generation strategy stores everything as an integer.
 * Consequently, we convert the value with "inttoptr" before loading
 * the value at the pointed-to memory location.
 */
llvm::Value* ASTDeRefExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  bool isLValue = lValueGen;

  if (isLValue) {
    // This flag is reset here so that sub-expressions are treated as r-values
    lValueGen = false;
  }
 
  Value *argVal = getPtr()->codegen();
  if (argVal == nullptr) {
    throw InternalError("failed to generate bitcode for the pointer");
  }

  // compute the address
  Value *address = Builder.CreateIntToPtr(argVal, Type::getInt64PtrTy(TheContext), "ptrIntVal");

  if (isLValue) {
    // For an l-value, return the address
    return address;
  } else {
    // For an r-value, return the value at the address
    return Builder.CreateLoad(address->getType()->getPointerElementType(), address, "valueAt");
  }
}

/* {field1 : val1, ..., fieldN : valN} record expression
 *
 * Builds an instance of the UberRecord using the declared fields
 */
llvm::Value* ASTRecordExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  //If this is an alloc, we calloc the record
  if(allocFlag){
    //Allocate the a pointer to an uber record
    auto *allocaRecord = Builder.CreateAlloca(ptrToUberRecordType);

    // Use Builder to create the calloc call using pre-defined callocFun
    auto sizeOfUberRecord = CurrentModule->getDataLayout().getStructLayout(uberRecordType)->getSizeInBytes();
    std::vector<Value *> callocArgs;
    callocArgs.push_back(oneV); 
    callocArgs.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), sizeOfUberRecord));
    auto *calloc = Builder.CreateCall(callocFun, callocArgs, "callocedPtr");

    //Bitcast the calloc call to theStruct Type
    auto recordPtr = Builder.CreatePointerCast(calloc, ptrToUberRecordType, "recordCalloc");

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

  //Return int64 pointer to the pointer to the record
  return Builder.CreatePtrToInt(recordPtr, Type::getInt64Ty(TheContext), "recordPtr");
  }
  else{
    //Allocate a the space for a uber record
    auto *allocaRecord = Builder.CreateAlloca(uberRecordType);

    //Codegen the fields present in this record and store them in the appropriate location
    //We do not give a value to fields that are not explictly set. Thus, accessing them is
    //undefined behavior
    for(auto const &field : getFields()){
      auto *gep = Builder.CreateStructGEP(allocaRecord->getAllocatedType(), allocaRecord, fieldIndex[field->getField()], field->getField());
      auto value = field->codegen();
      Builder.CreateStore(value, gep);
    }
    //Return int64 pointer to the record since all variables are pointers to ints
    return Builder.CreatePtrToInt(allocaRecord, Type::getInt64Ty(TheContext), "record");
  }
}

/* field : val field expression
 *
 * Expression for generating the code for the value of a field
 */
llvm::Value* ASTFieldExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  return this->getInitializer()->codegen();
} // LCOV_EXCL_LINE

/* record.field Access Expression
 *
 * In an l-value context this returns the location of the field being accessed
 * In an r-value context this returns the value of the field being accessed
 */
llvm::Value* ASTAccessExpr::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  bool isLValue = lValueGen;

  if (isLValue) {
      // This flag is reset here so that sub-expressions are treated as r-values
      lValueGen = false;
  }

  //Get current field and check if it exists
  auto currField = this->getField();
  if(fieldIndex.count(currField) == 0){
    throw InternalError("This field doesn't exist");
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
  throw InternalError("Declarations do not emit code");
}

llvm::Value* ASTDeclStmt::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  // The LLVM builder records the function we are currently generating
  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  AllocaInst *localAlloca = nullptr;

  // Register all variables and emit their initializer.
  for (auto l : getVars()) {
    localAlloca = CreateEntryBlockAlloca(TheFunction, l->getName());

    // Initialize all locals to "0"
    Builder.CreateStore(zeroV, localAlloca);

    // Remember this binding.
    NamedValues[l->getName()] = localAlloca;
  }

  // Return the body computation.
  return localAlloca;
}  // LCOV_EXCL_LINE

llvm::Value* ASTAssignStmt::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  // trigger code generation for l-value expressions
  lValueGen = true;
  Value *lValue = getLHS()->codegen();
  lValueGen = false;

  if (lValue == nullptr) {
    throw InternalError("failed to generate bitcode for the lhs of the assignment");
  }

  Value *rValue = getRHS()->codegen();
  if (rValue == nullptr) {
    throw InternalError("failed to generate bitcode for the rhs of the assignment");
  }

  return Builder.CreateStore(rValue, lValue);
}  // LCOV_EXCL_LINE

llvm::Value* ASTBlockStmt::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  Value *lastStmt = nullptr;

  for (auto const &s : getStmts()) {
    lastStmt = s->codegen();
  }

  // If the block was empty return a nop
  return (lastStmt == nullptr) ? Builder.CreateCall(nop) : lastStmt;
}  // LCOV_EXCL_LINE

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
  LOG_S(1) << "Generating code for " << *this;

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
      throw InternalError("failed to generate bitcode for the conditional"); // LCOV_EXCL_LINE
    }

    // Convert condition to a bool by comparing non-equal to 0.
    CondV = Builder.CreateICmpNE(CondV, ConstantInt::get(CondV->getType(), 0), "loopcond");

    Builder.CreateCondBr(CondV, BodyBB, ExitBB);
  }

  // Emit loop body
  {
    TheFunction->getBasicBlockList().push_back(BodyBB);
    Builder.SetInsertPoint(BodyBB);

    Value *BodyV = getBody()->codegen();
    if (BodyV == nullptr) {
      throw InternalError("failed to generate bitcode for the loop body"); // LCOV_EXCL_LINE
    }

    Builder.CreateBr(HeaderBB);
  }

  // Emit loop exit block.
  TheFunction->getBasicBlockList().push_back(ExitBB);
  Builder.SetInsertPoint(ExitBB);
  return Builder.CreateCall(nop);
}  // LCOV_EXCL_LINE

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
  LOG_S(1) << "Generating code for " << *this;

  Value *CondV = getCondition()->codegen();
  if (CondV == nullptr) {
    throw InternalError("failed to generate bitcode for the condition of the if statement");
  }

  // Convert condition to a bool by comparing non-equal to 0.
  CondV = Builder.CreateICmpNE(CondV, ConstantInt::get(CondV->getType(), 0), "ifcond");

  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  /*
   * Create blocks for the then and else cases.  The then block is first so
   * it is inserted in the function in the constructor. The rest of the blocks
   * need to be inserted explicitly into the functions basic block list
   * (via a push_back() call).
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
      throw InternalError("failed to generate bitcode for the then block"); // LCOV_EXCL_LINE
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
        throw InternalError("failed to generate bitcode for the else block"); // LCOV_EXCL_LINE
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
}  // LCOV_EXCL_LINE

llvm::Value* ASTOutputStmt::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  if (outputIntrinsic == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(TheContext));
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), oneInt, false);
    outputIntrinsic =
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                               "_tip_output", CurrentModule.get());
  }

  Value *argVal = getArg()->codegen();
  if (argVal == nullptr) {
    throw InternalError("failed to generate bitcode for the argument of the output statement");
  }

  std::vector<Value *> ArgsV(1, argVal);

  return Builder.CreateCall(outputIntrinsic, ArgsV);
}

llvm::Value* ASTErrorStmt::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  if (errorIntrinsic == nullptr) {
    std::vector<Type *> oneInt(1, Type::getInt64Ty(TheContext));
    auto *FT = FunctionType::get(Type::getInt64Ty(TheContext), oneInt, false);
    errorIntrinsic = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                            "_tip_error", CurrentModule.get());
  }

  Value *argVal = getArg()->codegen();
  if (argVal == nullptr) {
    throw InternalError("failed to generate bitcode for the argument of the error statement");
  }

  std::vector<Value *> ArgsV(1, argVal);

  return Builder.CreateCall(errorIntrinsic, ArgsV);
}

llvm::Value* ASTReturnStmt::codegen() {
  LOG_S(1) << "Generating code for " << *this;

  Value *argVal = getArg()->codegen();
  return Builder.CreateRet(argVal);
} // LCOV_EXCL_LINE
