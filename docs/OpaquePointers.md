# Quick summary of Opaque Pointers and how that affects TIPC.

LLVM IR traditionally used pointers with explicit pointee types (e.g., `i32*`). LLVM's opaque pointer project aims to
simplify this by replacing pointers with a generic opaque type (`ptr`). This change is driven by the need to address
issues related to the complexity and limitations of explicit pointee types in LLVM's optimization processes and the
challenges they pose to frontends and LLVM passes.

### Changes in LLVM15+:

- **Opaque Pointer Type**:
    - Replaces explicit pointee types (e.g., `i32*`) with a generic `ptr` type.
    - Instructions now include type arguments directly (e.g., `load i64, ptr %p` instead of `load i64* %p`).

- **Address Spaces**:
    - Address spaces remain unchanged and are used as before for differentiating pointer types relevant to
      architecture-specific lowering.

- **Migration Instructions**:
    - **Removal of Functions**: Functions like `PointerType::getElementType()` and `Type::getPointerElementType()` are
      deprecated.

- **Frontends Adjustments**:
    - Frontends need to independently manage pointee types. Certain C API functions related to pointer types are
      deprecated or replaced.

## Changes in TIPC

> [Old Code: [commit 0836f7a](https://github.com/matthewbdwyer/tipc/blob/0836f7a3999c9a6e4a8093ded668c3d6c2fbee16/src/codegen/CodeGenFunctions.cpp)] - [New Code: [commit 540f37f ](https://github.com/matthewbdwyer/tipc/blob/68526bfa1746b58884e50c47c6812184c8201212/src/codegen/CodeGenFunctions.cpp)]

The significant changes are in the way we handle types and function table
in [CodeGenFunctions.cpp](../src/codegen/CodeGenFunctions.cpp).

#### Major Changes

- **Function table structure:**
    - From array of "Function type pointer", for example - `i64()*`, to `ptr` pointer type.
      As an upside, the new function table looks cleaner.

Following is the function table of `test/system/iotests/fib.tip`, pre- and post-change -

```ll
@_tip_ftable = internal constant [2 x i64 ()*] [i64 ()* bitcast (i64 (i64)* @fib to i64 ()*), i64 ()* @_tip_main]
```

to

```ll
@_tip_ftable = internal constant [2 x ptr] [ptr @fib, ptr @_tip_main]
```

- **Function pointer changed from typed to opaque:**

```c++
auto *genFunPtrType = PointerType::get(FunctionType::get(Type::getInt64Ty(TheContext), None, false), 0);
```

to

```c++
auto *FunctionOpaquePtrType = llvm::PointerType::get(llvmContext, 0);
```

- **Load types adjustments to account for `ptr -> <target_type>`:**

```c++
auto *inVal = Builder.CreateLoad(gep->getType()->getPointerElementType(), gep, "tipinput" + std::to_string(argIdx++));
```

```c++
auto *inVal = irBuilder.CreateLoad(llvm::Type::getInt64Ty(llvmContext), gep, "tipinput" + std::to_string(argIdx++));
```

- **Function call resolutions:**

```c++
auto *gep = Builder.CreateInBoundsGEP(tipFTable->getValueType(), tipFTable, indices, "ftableidx");

// Load the function pointer
auto *genericFunPtr = Builder.CreateLoad(gep->getType()->getPointerElementType(), gep, "genfptr");

// Compute the specific function pointer type based on the actual parameter list.
...
auto *funType = FunctionType::get(Type::getInt64Ty(TheContext), actualTypes, false);
auto *funPtrType = PointerType::get(funType, 0);

// Bitcast the function pointer to the call-site determined function type
auto *castFunPtr = Builder.CreatePointerCast(genericFunPtr, funPtrType, "castfptr");

...

return Builder.CreateCall(funType, castFunPtr, argsV, "calltmp");
```

Is now simpler with fewer operations, because we are not using typed pointers anymore, we don't need `PointerCast`
or `BitCast` -

```c++
auto *gep = irBuilder.CreateInBoundsGEP(tipFunctionTable->getValueType(), tipFunctionTable, indices, "ftableidx");

// Load the function pointer
auto *functionPointer = irBuilder.CreateLoad(llvm::PointerType::get(llvmContext, 0), gep, "genfptr");
...

auto *funType = llvm::FunctionType::get(llvm::Type::getInt64Ty(llvmContext), actualTypes, false);
...

return irBuilder.CreateCall(funType, functionPointer, argsV, "calltmp");
```

* **Using new inserters instead of vector push_backs:**

```c++
TheFunction->getBasicBlockList().push_back(ExitBB);
```

```c++
TheFunction->insert(TheFunction->end(), BodyBB);
```

#### Other minor changes in the code -

* Renamed variables so that they have more descriptive names.
* std::vector.reserve() calls before vector allocation loops to improve performance.
* making reference only variables `const&`, so that `*getFunction(std::string functionName)` is
  now `*getFunction(const std::string& functionName)`. This avoids unnecessary copies.