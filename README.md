[![Build Status](https://travis-ci.com/matthewbdwyer/tipc.svg?branch=devel)](https://travis-ci.com/matthewbdwyer/tipc)

[![codecov.io](https://codecov.io/gh/matthewbdwyer/tipc/coverage.svg?branch=devel)](https://codecov.io/gh/matthewbdwyer/tipc?branch=devel)

# tipc
A compiler from TIP to llvm bitcode

## TIP Language, Interpreter, and Analyzers

TIP is a "Tiny Imperative Programming" language developed by Anders Moeller and Michael I. Schwartzbach for the [Static Program Analysis](https://cs.au.dk/~amoeller/spa/ "Static Program Analysis") lecture notes that they developed for graduate instruction at Aarhus University.

Accompanying those notes is a [Scala implementation](https://github.com/cs-au-dk/TIP/) that provides a number of static analysis implementations and interpreter-based evaluators.

This project implements `tipc` which compiles TIP programs into LLVM bitcode.
Linking that bitcode with the [runtime library](./rtlib) routines and
standard libraries will produce an executable.

## Dependencies

`tipc` was developed and tested on Ubuntu 18.04 LTS.
It depends on java8, cpp, cmake, antlr4, clang, and llvm which were installed using:
  1. `apt install openjdk-8-jdk-headless`
  2. `apt install cmake pkg-config uuid-dev`
  3. `apt install antlr4`
  4. `apt install libllvm-10-ocaml-dev libllvm10 llvm-10 llvm-10-dev llvm-10-doc llvm-10-examples llvm-10-runtime`
  5. `apt install clang-10`

We use the `llvm-10` and `clang-10` packages, but other versions of may work as well.  



## Building the executable

This project uses CMake to manage the build process.  We follow the cmake build model for the [ANTLR4 Cpp target](https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake).  The `cmake` directory stores ANTLR4 related cmake files. 

`tipc` has a `CMakeLists.txt` file in the `src` directory.  This file configures the ANTLR4 and Cpp build process, e.g., source files, targets, libraries, etc.  You will need to adjust the path to the jar file that ANTLR4 uses (`antlr-4.8-complete.jar`) which is hardcoded in `CMakeLists.txt`.   Note that if the normal install of `antlr4` doesn't provide you with this file, then you can download it from the [ANTLR4 download site](https://www.antlr.org/download/antlr-4.8-complete.jar) and place it wherever you like, e.g., `$HOME/lib`.

You need to create a `build` directory within which you will build `tipc`.  To get started you should create that, empty, build directory if it doesn't exist.  All of the generated runtime files for ANTLR4 will be built the first time and stored in the build directory; this may take some time.

To build a tipc:
  1. `mkdir build`
  2. `cd build`
  3. `cmake ../src`
  4. `make`

NB: You may see warnings related to ignored type attribute ATN that are due to some 
type gymnastics in the current implementation of the ANTLR4 CPP Visitor implementation.

During development you need only run steps 1 and 2 a single time, unless you modify the CmakeLists.txt file.  Just run make in the build directory to rebuild after making changes to your tool source.

Note that the `tipg4` directory has a standalone ANTLR4 grammar.  It's README describes how to build it and run it using `grun`.

## Limitations

TIP does not perform type checking.  Instead it relies on running the Scala TIP system to perform this check.  `tipc` relies on the fact that the types are correct and it casts values based on the operators, e.g., an operator that expects a pointer has its operand cast to a pointer.  This can work because TIP has a limited set of types and all of them can be represented as an `int64_t`.  This results in sub-optimal code because there are lots of `inttoptr` and `ptrtoint` casts in the generated LLVM bitcode.

TIP records are not implemented (yet).  Extending `tipc` to support records can still use the above scheme since operators that access records are not overloaded with any other type and records are always heap allocated in TIP and, thus, that address can be represented as a pointer/`int64_t`.

Note that other extensions of TIP, e.g., adding floats, would require having type annotations in `tipc`.  These could be constructed in a simple type annotation pass, as opposed to TIP Scala's type inference analysis, if the types of functions and declared variables were known.   Conveniently, the Scala TIP compiler can emit type annotations for functions and declared variables; run `./tip -types` and use the file with the `.ttip` suffix that is written to the `out` directory.

## Tests

The implementation has a small set of rudimentary tests.  These tests do not accept input, rather they hard code values to exercise the generated code.  Tests are judged correct for `tipc` if the output produced matches that produced when running `./tip -run` for the Scala implementation.  The Scala implementation produces more output than `tipc`, due to messages from running it under `sbt`.  Consequently, test output is only judged relative to values produced by program `output` and `error` statements.  

The script [difftest.sh](./test/difftest.sh) takes the name of a `.tip` program and runs differential testing on it.  If your program reads from input the test script will hang.

To run the current set of differential tests:
  1. compile and build TIP in `~/TIP`
  2. create a directory `~/TIP/tipc` to hold test input files
  3. run `./build.sh` in `.../tipc/rtlib`
  4. run `./runtests.sh` in `.../tipc/test`

The ANTLR4 grammar is designed to make it possible to perform grammar-based fuzzing using a tool like [grammarinator](https://github.com/renatahodovan/grammarinator).  To make this interesting one must bias the fuzzing towards programs that are syntactically and type correct and that have no input statements.  An even more interesting set of generated tests would aggressively output the results of intermediate computations, e.g., after every assignment.  This is future work.

## Documentation

The TIP grammar, [tipg4](./tipg4/TIP.g4), is implemented using ANTLR4.  This grammar is free of any semantic actions, though it does use ANTLR4 rule features which allow for control over the tree visitors that form key parts of the compiler.  This allows the structure of the grammar to remain relatively clean, i.e., no grammar factoring or stratification needed.  Relative to the TIP Scala grammar, which is expressed as a PEG grammar, the ANTLR4 grammar consolidates some rules to facilitate the access to parsed structures in tree visitors.

The `tipc` compiler is pretty straightforward.  It includes a [parse tree visitor](./src/TIPtreeBuild.cpp) that constructs an [AST](./src/TIPtree.h).  The compiler implements two passes over the AST: one to [generate LLVM bitcode](./src/TIPtreeGen.cpp) and one to [pretty print](.src/TIPtreePrint.cpp).   The [main](./src/tipcc.cpp) file parses command line options, chooses which pass to run, and if LLVM code is generated whether to run a set of LLVM passes to improve the bitcode (on by default).

`tipc` only produces a bitcode file, `.bc`.  You need to link it with the [runtine library](./rtlib/tip_rtlib.c) which define the processing of command line arguments, which is non-trivial for TIP, establish necessary runtime structures, and implement IO routines.  A [script](./test/build.sh) is available to statically link binaries compiled by `tipc`.

To understand this code, and perhaps extend it, you will want to become familiar with the [core LLVM classes](http://llvm.org/docs/ProgrammersManual.html#the-core-llvm-class-hierarchy-reference).  It can be difficult to absorb all of the information in this type of documentation just by reading it.  A goal-directed strategy where you move back and forth between reading code and reading this documentation seems to work well for many people.

If you are familiar with the [LLVM tutorial](https://llvm.org/docs/tutorial/) you will see its influence on this compiler which leverages idioms, strategies, and code fragments from the tutorial.  The LLVM tutorials are a great starting point for understanding the APIs in the context of compiling.

There is lots of great advice about using LLVM available:
  * https://www.cs.cornell.edu/~asampson/blog/llvm.html
  * the [LLVM Programmer's Manual](http://llvm.org/docs/ProgrammersManual.html) is a key resource
  * someone once told me to just use a search engine to find the LLVM APIs and its a standard use case for me, e.g., I don't remember where the docs are I just search for `llvm irbuilder`
  * LLVM has some nuances that take a bit to understand.  For instance, the [GEP](https://llvm.org/docs/GetElementPtr.html) instruction, which `tipc` uses quite a bit given that it emits calls through a function table.
  
