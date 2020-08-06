[![Build Status](https://travis-ci.com/matthewbdwyer/tipc.svg?branch=devel)](https://travis-ci.com/matthewbdwyer/tipc)
[![codecov.io](https://codecov.io/gh/matthewbdwyer/tipc/coverage.svg?branch=devel)](https://codecov.io/gh/matthewbdwyer/tipc?branch=devel)

tipc
=========
A compiler from TIP to llvm bitcode


## TIP Language, Interpreter, and Analyzers

TIP is a "Tiny Imperative Programming" language developed by Anders Moeller and Michael I. Schwartzbach for the [Static Program Analysis](https://cs.au.dk/~amoeller/spa/ "Static Program Analysis") lecture notes that they developed for graduate instruction at Aarhus University.

Accompanying those notes is a [Scala implementation](https://github.com/cs-au-dk/TIP/) that provides a number of static analysis implementations and interpreter-based evaluators.

This project implements `tipc` which compiles TIP programs into LLVM bitcode.  Linking that bitcode with the [runtime library](./rtlib) routines and standard libraries will produce an executable.

## Dependencies

`tipc` is implemented in C++17 and depends on a number of tools and packages, e.g., [ANTLR4](https://www.antlr.org), [Catch2](https://github.com/catchorg/Catch2), [CMake](https://cmake.org/), [Doxygen](https://www.doxygen.nl/), [Java](https://www.java.com), [LLVM](https://www.llvm.org).  To simplify dependency management the project provides a [bootstrap](./bootstrap) script to install all of the required dependencies on linux ubuntu and mac platforms.

## Building tipc

The project use [Travis CI](https://travis-ci.org) for building and testing and [CodeCov](https://codecov.io)] for reporting code coverage.  The [.travis.yml](.travis.yml)] file provides details of this process.  If you would prefer to build and test manually then read on.

After cloning this repository you can build the compiler by moving to into the top-level directory and issuing these commands:
  1. `./bootstrap`
  2. `. ~/.bashrc`
  3. `mkdir build`
  4. `cd build`
  5. `cmake ../src`
  6. `make`

The build process will download an up to date version of ANTLR4 if needed, build the C++ target for ANTLR4, and then build all of `tipc` including its substantial body of unit tests.  This may take some time - to speed it up use multiple threads in the `make` commend, e.g., `make -j6`.

You may see some warnings, e.g.,
  * CMake policy warnings
  * compile warnings for ignored type attribute ATN

These are expected in the current version of the project; we will work to resolve them in future releases.

The project includes a number of unit tests grouped into several executables.  To run them issue the following commands:
  1. `cd build`
  2. `./test/unit/frontend/frontend_unit_tests`
  3. `./test/unit/semantic/semantic_unit_tests`
  4. `./test/unit/semantic/types/typeinference_unit_tests`

The project also includes a number of system tests.  These are TIP programs that have built in test oracles that check for the expected results.  Unlike the unit tests, these test require linking with the runtime library so you need to build that.  To run system tests issue the following commands:
  1. `cd rtlib`
  2. `./build.sh`
  3. `cd ../test/system`
  4. `./run.sh`

All of the tests should pass.

## Working with tipc

During development you need only run build steps 1 through 5 a single time, unless you modify some `CMakeLists.txt` file.  Just run make in the build directory to rebuild after making changes to the source.

If you do need to add a source file then you will have to edit the appropriate CMakeLists.txt file to add it.  In this case, you should:
  1. `cd build`
  2. `rm CMakeCache.txt`
  3. `cmake ..`

which will regenerate the makefiles that you can then run, by typing `make`, to build.

Note that the `tipg4` directory has a standalone ANTLR4 grammar.  It's README describes how to build it in isolation and run it using the ANTLR4 jar file.

## Documentation

The TIP grammar, [tipg4](./tipg4/TIP.g4), is implemented using ANTLR4.  This grammar is free of any semantic actions, though it does use ANTLR4 rule features which allow for control over the tree visitors that form key parts of the compiler.  This allows the structure of the grammar to remain relatively clean, i.e., no grammar factoring or stratification needed.  

The `tipc` compiler is has a pretty classic design.  It is comprised of four phases:
 * [frontend](./src/frontend) takes care of parsing, constructing an AST representation, and pretty printing
 * [semantic analysis](./src/semantic) that performs assignability, symbol, and type checking
 * [code generation](./src/codegen) that produces LLVM bitcode from an AST and emits a binary
 * [optimization](./src/optimizer) that runs a few LLVM optimization passes to improve the bitcode

Doxygen [documentation](https://matthewbdwyer.github.io/tipc) for the project is available for the project.  The documentation is a work in progress and will improve over time..

The `tipc` driver program only produces a bitcode file, `.bc`.  You need to link it with the [runtime library](./rtlib/tip_rtlib.c) which define the processing of command line arguments, which is non-trivial for TIP, establish necessary runtime structures, and implement IO routines.  A [script](./test/system/build.sh) is available to link binaries compiled by `tipc` with the runtime library.

## Goals and Plans

The goal of this project is to provide a starting point for project work in an undergraduate compilers course.  As such it similar to lots of other compiler projects, but there are some differences.

First and foremost, the TIP language includes a number of rich features, e.g., high-order functions, and type inference, and the `tipc` compiler targets LLVM - a key component of a production compiler infrastructure.  These choices are intentional and while they create some challenges the project is intended to help demystify complex language features, e.g., parametric polymorphism, by illustrating how they can be realized.

Second, the project attempts to use modern software development practices, e.g., Doxygen for in-code documentation, unit testing with Catch2, continuous integration with Travis CI, and code coverage with `lcov`.  

Third, the project intentionally makes heavy use of the [Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern) which is quite appropriate in the context of a compiler.  Our use of it is intended to demonstrate how this type of abstract design element in a system can yield conceptually simplicity and savings in development.   The project currently uses 6 visitors that extend [ASTVisitor](./src/frontend/ast/ASTVisitor.h) and another visitor from ANTLR4.

Finally, the project is implemented in C++17 using modern features.  For example, all memory allocation uses smart pointers, we use unique pointers where possible and shared pointers as well, to realize the [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) pattern.  Again this presents some challenges, but addressing them is illustrated in the `tipc` code base and hopefully they provide a good example for students.  

The project is a work-in-progress in the sense that we are planning to perform corrective maintenance, as needed, as well as perfective maintenance.  For the latter, we expect to make a new release of the project in early August every year.  This release will focus on improving the use of modern C++, as we come to better understand the best practices for C++20, and to incorporate better design principles, patterns, and practices.

## Limitations

Other than issues related to the efficiency of the code that it generates, the `tipc` compiler has two limitations.

First, it implements a variant of the TIP language semantics with respect to records.   `tipc` implements the unification-based type inference algorithm used in the [Scala implementation](https://github.com/cs-au-dk/TIP/).  This yields a type system that ensures that any expression appearing in the record position of a field access expression is in fact a record, but it does not infer precise record typing.  Instead the strategy used is to define an *uber* record that consists of all of the fields referenced across the program.  Code generation for records will allocate an uber record, default initialize all of its fields, and then explicitly initialize fields present in a record expression.   This can lead to some unexpected behavior.  Consider this TIP program:
```
main() { var r; r = {g:1}; return access(r); }
access(r) { return r.f; }
```
The record expression, `{g:1}`, forces the uber record to contain a field `g`, and the access expression `r.f`, forces the presence of field `f`.  The record expression default initializes `f` to `0` and this is the value that is accessed and returned from the call to `access` and then from `main`.  One might prefer that this program yield a type error, but that would require a more sophisticated type system.  We chose not to implement that to manage the complexity of what is primarily a pedagogical project.

Second, TIP allows memory allocation, and in fact records are allocated implicitly on the heap, yet its runtime system does not include a garbage collector.  It's an easy matter to write a TIP program that leaks memory:
```
main(max) {
  var i, h;
  i = 0;
  h = null;
  while (max > i) {
    h = alloc h;
    i = i + 1;
  }
  return 0;
}
```
Running this program with a large enough input yields the expected result:
```
$ ./leak 1000
Program output: 0
$ ./leak 1000000000000000
Killed
```
Incorporating a garbage collector is a possible future extension to the runtime library.

## Resources
To fully understand this project quite a bit of background is required.
We collect a number of resources that we think can be helpful in that regard.

### C++ Resources
If you find yourself unfamiliar with certain aspects of the C++ programming
language we encourage you to explore the *Back To Basics* videos that have
been presented at [CppCon](https://cppcon.org/). Provided below are links
to a number of these videos, as well as to other resources that are relevant 
to this project.

#### Move Semantics
+ [Move Semantics (part 1 of 2)](https://youtu.be/St0MNEU5b0o)
+ [Move Semantics (part 2 of 2)](https://youtu.be/pIzaZbKUw2s)

#### Value Categories
+ [Understanding Value Categories](https://youtu.be/XS2JddPq7GQ)
+ [“New” Value Terminology](https://www.stroustrup.com/terminology.pdf)

#### Smart pointers
+ [Smart Pointers](https://youtu.be/xGDLkt-jBJ4)

### CMake Resources
+ [CMake docs](https://cmake.org/cmake/help/v3.7/)
+ [More Modern CMake](https://youtu.be/y7ndUhdQuU8)
+ [Oh No! More Modern CMake](https://youtu.be/y9kSr5enrSk)

### Catch2 and Unit Testing Resources
+ [Catch2 docs](https://github.com/catchorg/Catch2/tree/master/docs)
+ [Modern C++ Testing with Catch2](https://youtu.be/Ob5_XZrFQH0)

### LLVM Resources

To understand this code, and perhaps extend it, you will want to become familiar with the [core LLVM classes](http://llvm.org/docs/ProgrammersManual.html#the-core-llvm-class-hierarchy-reference).  It can be difficult to absorb all of the information in this type of documentation just by reading it.  A goal-directed strategy where you move back and forth between reading code and reading this documentation seems to work well for many people.

If you are familiar with the [LLVM tutorial](https://llvm.org/docs/tutorial/) you will see its influence on this compiler which leverages idioms, strategies, and code fragments from the tutorial.  The LLVM tutorials are a great starting point for understanding the APIs in the context of compiling.

There is lots of great advice about using LLVM available:
  * https://www.cs.cornell.edu/~asampson/blog/llvm.html
  * the [LLVM Programmer's Manual](http://llvm.org/docs/ProgrammersManual.html) is a key resource
  * someone once told me to just use a search engine to find the LLVM APIs and its a standard use case for me, e.g., I don't remember where the docs are I just search for `llvm irbuilder`
  * LLVM has some nuances that take a bit to understand.  For instance, the [GEP](https://llvm.org/docs/GetElementPtr.html) instruction, which `tipc` uses quite a bit given that it emits calls through a function table.
  
