[![Build Status](https://github.com/matthewbdwyer/tipc/actions/workflows/build-and-test.yml/badge.svg?branch=main)](https://github.com/matthewbdwyer/tipc/actions/workflows/build-and-test.yml)
[![codecov.io](https://codecov.io/gh/matthewbdwyer/tipc/coverage.svg?branch=main&flag=codecoverage)](https://codecov.io/gh/matthewbdwyer/tipc?branch=main)
[![codecov.io](https://img.shields.io/endpoint?logo=codecov&logoColor=blue&url=https%3A%2F%2Fmatthewbdwyer.github.io%2Ftipc%2Fdoccoverage.json)](https://codecov.io/gh/matthewbdwyer/tipc?branch=main)

tipc
=========
A compiler from TIP to llvm bitcode


## TIP Language, Interpreter, and Analyzers

TIP is a "Tiny Imperative Programming" language developed by Anders M&#248;ller and Michael I. Schwartzbach for the [Static Program Analysis](https://cs.au.dk/~amoeller/spa/ "Static Program Analysis") lecture notes that they developed for graduate instruction at Aarhus University.

Accompanying those notes is a [Scala implementation](https://github.com/cs-au-dk/TIP/) that provides a number of static analysis implementations and interpreter-based evaluators.

This project implements `tipc` which compiles TIP programs into LLVM bitcode.  Linking that bitcode with the [runtime library](./rtlib) routines and standard libraries will produce an executable.

## Dependencies

`tipc` is implemented in C++17 and depends on a number of tools and packages, e.g., [ANTLR4](https://www.antlr.org), [Catch2](https://github.com/catchorg/Catch2), [CMake](https://cmake.org/), [Doxygen](https://www.doxygen.nl/), [loguru](https://github.com/emilk/loguru), [Java](https://www.java.com), [LLVM](https://www.llvm.org).  To simplify dependency management the project provides a [bootstrap](bin/bootstrap.sh) script to install all of the required dependencies on linux ubuntu and mac platforms.

## Building tipc

The project uses [GitHub Actions](https://docs.github.com/en/actions) for building and testing and [CodeCov](https://codecov.io) for reporting code and documentation coverage.  The [build-and-test.yml](.github/workflows/build-and-test.yml) file provides details of this process.  If you would prefer to build and test manually then read on.

After cloning this repository you can build the compiler by moving to into the top-level directory and issuing these commands:
  1. `./bin/bootstrap.sh`
  2. `. ~/.bashrc`
  3. `mkdir build`
  4. `cd build`
  5. `cmake ..`
  6. `make`

The build process will download an up to date version of ANTLR4 if needed, build the C++ target for ANTLR4, and then build all of `tipc` including its substantial body of unit tests.  This may take some time - to speed it up use multiple threads in the `make` command, e.g., `make -j6`.

You may see some warnings, e.g.,
  * CMake policy warnings
  * compile warnings for ignored type attribute ATN

These are expected in the current version of the project; we will work to resolve them in future releases.

When finished the `tipc` executable will be located in `build/src/`.  You can copy it to a more convenient location if you like, but a number of scripts in the project expect it to be in this location so don't move it.

The project includes more than 300 unit tests grouped into several executables. The project also includes more than 70 system tests. These are TIP programs that have built in test oracles that check for the expected results. For convenience, there is a `runtests.sh` script provided in the `bin` directory.  You can run this script to invoke the entire collection of tests. See the `README` in the bin directory for more information.  

All of the tests should pass.

### Ubuntu Linux

Our continuous integration process builds on both Ubuntu 18.04 and 20.04, so these are well-supported.  We do not support other linux distributions, but we know that people in the past have ported `tipc` to different distributions. 

### Mac OS

Our continuous integration process builds on Mac OS 10.15 and we regularly build and test on Mac OS 11.4, so modern versions of Mac OS are well-supported.  `tipc` builds on both Intel and Apple Silicon, i.e., Apple's M1 ARM processor.  

### Windows Subsystem for Linux

If you are using a Windows machine, tipc can be built in the Windows Subsystem for Linux (WSL). [Here](https://docs.microsoft.com/en-us/windows/wsl/install-win10#update-to-wsl-2) are instructions to install WSL and upgrade to WSL2. It is highly recommended to upgrade to WSL2. Once installed, you should install
[Ubuntu 20.04](https://docs.microsoft.com/en-us/windows/wsl/install-win10#update-to-wsl-2). Once finished, you can open a virtual instance of Ubuntu and follow 
the instructions above to build tipc. 

You may recieve an error saying "No CMAKE_CXX_COMPILER could be found" when running `cmake ..`. If this is the case, you should install g++ with the command: `sudo apt-get install g++`.

## Using tipc

The `tipc` compiler has a limited set of options available through the `--help` flag.
```
OVERVIEW: tipc - a TIP to llvm compiler

USAGE: tipc [options] <tip source file>

OPTIONS:

Generic Options:

  --help                 - Display available options (--help-hidden for more)
  --help-list            - Display list of available options (--help-list-hidden for more)
  --version              - Display the version of this program

tipc Options:
Options for controlling the TIP compilation process.

  --asm                  - emit human-readable LLVM assembly language instead of LLVM Bitcode
  --da=<ast output file> - dump the ast to a file in the dot syntax
  --do                   - disable bitcode optimization
  --log=<logfile>        - log all messages to logfile (enables --verbose)
  --pp                   - pretty print
  --ps                   - print symbols
  --pt                   - print symbols with types (supercedes --ps)
  --pcg           - print call graph
  --verbose=<int> - enable log messages (Levels 1-3) 
                     Level 1 - Symbols being added to the symbol table and type constraints being generated for the type solvers.
                     Level 2 - Level 1 and type constraints being unified.
                     Level 3 - Level 2 and type constraints being added and searched for in the type graph
```
By default it will accept a `.tip` file, parse it, perform a series of semantic analyses to determine if it is a legal TIP program, generate LLVM bitcode, and emit a `.bc` file which is a binary encoding of the bitcodes.  You can see a human readable version of the bitcodes by running `llvm-dis` on the `.bc` file.

To produce an executable version of a TIP program, the `.bc` file must be linked with the bitcode for [tip_rtlib.c](rtlib/tip_rtlib.c).  Running the `build.sh` script in the [rtlib](rtlib) directory once will create that library bitcode file.

The link step is performed using `clang` which will include additional libraries needed by [tip_rtlib.c](rtlib/tip_rtlib.c).  

For convenience, we provide a script [build.sh](bin/build.sh) that will compile the tip program and perform the link step.  The script can be used within this git repository, or if you define the shell variable `TIPDIR` to the path to the root of the repository you can run it from any location as follows:
```
$ cd
$ more hello.tip
main() { return 42; }
$ $HOME/tipc/bin/build.sh hello.tip
$ ./hello
Program output: 42
$ $HOME/tipc/bin/build.sh -pp -pt hello.tip
main() 
{
  return 42;
}

Functions : {
  main : () -> int
}

Locals for function main : {

}
```

## Working with tipc

During development you need only run build steps 1 through 5 a single time, unless you modify some `CMakeLists.txt` file.  Just run `make` in the build directory to rebuild after making changes to the source.

If you do need to add a source file then you will have to edit the appropriate `CMakeLists.txt` file to add it.  In this case, you should:
  - `cd build`
  - `rm CMakeCache.txt`
  - `cmake ..`

which will regenerate the makefiles that you can then run, by typing `make`, to build.

Note that the `tipg4` directory has a standalone ANTLR4 grammar.  It's README describes how to build it in isolation and run it using the ANTLR4 jar file.

### Logging Messages
When working on the tipc compiler, it may be helpful to enable logging messages when testing your changes on programs. We have inserted logging messages using loguru. These can be turned using the flag `--verbose [x]` where x is a number between 1-3. These messages get more verbose as you increase x. The first setting shows when symbols are added to the symbol table and when type constraints are generated for the type solver. The second setting shows the previous information and type constraints being unified. The third setting shows types being search for and added into the type graph. When adding to theses features, you can add logging messages by adding a line `LOG_S(x)` where x is an integer to describe the level of log verbosity you want. You can use the existing levels or make new levels.

### The bin directory
To facilitate development of `tipc` we have collected a number of helper scripts into the `bin` directory of the project. Among them are scripts to run the entire test bed (`runtests.sh`), to run a code coverage analysis (`gencov.sh`), and to generate the project documentation (`gendocs.sh`).  Please see the `README` in the bin directory for example usages.  

### Visual Studio Code
[Visual Studio Code](https://code.visualstudio.com/) (VSCode) is a popular, open source text editor. The community has created many extensions that make it far more powerful than meerly a text editor. As a result, developers using VSCode get many of the benefits of a normal IDE with less overhead. Using VSCode, you can build and develop tipc with or without the command line.

#### Set-Up
0. Follow steps 1-4 of the building tipc if you have not already
1. Install VSCode using instructions listed for a supported operating system: [macOS](https://code.visualstudio.com/docs/setup/mac), [Linux](https://code.visualstudio.com/docs/setup/linux), [Windows](https://code.visualstudio.com/docs/setup/windows).
2. Bring up the extension marketplace using Ctrl+Shift+X or clicking the four squares on the side bar
3. Search for C++ to install the offical C/C++ extension produced by Microsoft
4. Open the project in VSCode using File->Open or simply type the command "code [path/to/tipc]"
5. If you are planning on building using the command line, proceed with steps 5-6 of building tipc, otherwise install the CMake Tools in the extension marketplace and proceed.
6. Open the Command Palette (Ctrl+Shift+P) and run CMake: Select a Kit. Select clang++-11 from the listed compilers
7. Open the Command Palette (Ctrl+Shift+P) and run CMake: Select Variant. Select the Debug variant
8. Open the Command Palette (Ctrl+Shift+P) and run CMake: Configure
9. Build the project using the CMake: Build command in the Command Palette (Ctrl+Shift+P) or select the Build button on the status bar at the bottom of VSCode.

Whenever you make changes to the project, simply repeat step 9. If you have to make changes to the CMakeLists, repeat both 8 and 9. For more information, follow [this tutorial](https://code.visualstudio.com/docs/cpp/cmake-linux) on using CMake and VSCode.


### CLion

[CLion](https://www.jetbrains.com/clion/) is a C and C++ IDE that can be used to develop and build tipc. CLion can be installed with the JetBrains suite of tools, or as a standalone tool [here](https://www.jetbrains.com/help/clion/installation-guide.html#standalone). Once installed, you can start a 30 day trial license or, as a student, you can get a free educational license [here](https://www.jetbrains.com/community/education/#students).

If you are building for the first time with CLion, follow the first two steps of the installation process to install any needed tipc dependencies. The project can now be built or rebuilt by clicking the "Build" button in the toolbar.

#### Troubleshooting
When working with CLion you might see the output become cluttered with messages about 
failing to merge `gcov` files. This happens when `gcov` files linger from previous
runs. To cleanup these messages, simply remove the `gcov` files. You can do
that quickly with the following one-liner.
```bash
find . -name '*gcda' -delete
```

## Documentation

The TIP grammar, [tipg4](./tipg4/TIP.g4), is implemented using ANTLR4.  This grammar is free of any semantic actions, though it does use ANTLR4 rule features which allow for control over the tree visitors that form key parts of the compiler.  This allows the structure of the grammar to remain relatively clean, i.e., no grammar factoring or stratification needed.  

The `tipc` compiler is has a pretty classic design.  It is comprised of four phases:
 * [frontend](./src/frontend) takes care of parsing, constructing an AST representation, and pretty printing
 * [semantic analysis](./src/semantic) that performs assignability, symbol, and type checking
 * [code generation](./src/codegen) that produces LLVM bitcode from an AST and emits a binary
 * [optimization](./src/optimizer) that runs a few LLVM optimization passes to improve the bitcode

Doxygen [documentation](https://matthewbdwyer.github.io/tipc) for the project is 
available for the project.  The documentation is a work in progress 
and will improve over time.

The `tipc` driver program only produces a bitcode file, `.bc`. You need to link it 
with the [runtime library](./rtlib/tip_rtlib.c) which define the processing of command 
line arguments, which is non-trivial for TIP, establish necessary runtime structures, 
and implement IO routines. A [script](./bin/build.sh) is available to link binaries 
compiled by `tipc` with the runtime library.

## Goals and Plans

The goal of this project is to provide a starting point for project work in an undergraduate compilers course.  As such it similar to lots of other compiler projects, but there are some differences.

First and foremost, the TIP language includes a number of rich features, e.g., high-order functions, and type inference, and the `tipc` compiler targets LLVM - a key component of a production compiler infrastructure.  These choices are intentional and while they create some challenges the project is intended to help demystify complex language features, e.g., parametric polymorphism, by illustrating how they can be realized.

Second, the project attempts to use modern software development practices, e.g. Doxygen for in-code documentation, unit testing with Catch2, continuous integration with GitHub Actions, and code coverage with `lcov`.  

Third, the project intentionally makes heavy use of the [Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern) which is quite appropriate in the context of a compiler.  Our use of it is intended to demonstrate how this type of abstract design element in a system can yield conceptual simplicity and savings in development.   The project currently uses 6 visitors that extend [ASTVisitor](./src/frontend/ast/ASTVisitor.h) and another visitor from ANTLR4.

Finally, the project is implemented in C++17 using modern features.  For example, all memory allocation uses smart pointers, we use unique pointers where possible and shared pointers as well, to realize the [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) pattern.  Again this presents some challenges, but addressing them is illustrated in the `tipc` code base and hopefully they provide a good example for students.  

The project is a work-in-progress in the sense that we are planning to perform corrective maintenance, as needed, as well as perfective maintenance.  For the latter, we expect to make a new release of the project in early August every year.  This release will focus on improving the use of modern C++, as we come to better understand the best practices for C++20, and to incorporate better design principles, patterns, and practices.

## Differences from TIP and Limitations

Other than issues related to the efficiency of the code that it generates, the `tipc` compiler has two limitations.

First, it implements a variant of the TIP language semantics in a few ways.  It implements the `!=` operator which allows us to conveniently write self-contained system tests and it implements the [C operator precedence rules](https://en.cppreference.com/w/c/language/operator_precedence), whereas the original TIP uses a few different rules.  This surfaces in the interplay between pointer dereference and field access.  An expression `*r.f` is interpreted as `*(r.f)` according t the C precedence rules and as `(*r).f` according to the [TIP Scala](https://github.com/cs-au-dk/TIP) implementation.  If in doubt, add parentheses to express your meaning.

The more important difference is the treatment of records.  `tipc` implements the unification-based type inference algorithm used in the [Scala implementation](https://github.com/cs-au-dk/TIP/).  This yields a type system that ensures that any expression appearing in the record position of a field access expression is in fact a record, but it does not infer precise record typing.  Instead the strategy used is to define an *uber* record that consists of all of the fields referenced across the program. Code generation for records will allocate an uber record and then explicitly initialize fields present in a record expression. If the record is being created via an `alloc` expresion, the fields that aren't explictly set will be set to a default value 0, as we allocate memory using the C function `calloc`. If the record is being created without explictly allocating memory for it, the fields that aren't explictly set will not be given any value, but may still be referenced. This can lead to some unexpected behavior.  Consider this TIP program:
```
main() { var r; r = {g:1}; return access(r); }
access(r) { return r.f; }
```
The record expression, `{g:1}`, forces the uber record to contain a field `g`, and the access expression `r.f`, forces the presence of field `f`. Because `r` is not being allocated using an `alloc` expression, access will return whatever value was in memory at the location of `r.f`. 
One might prefer that this program yield a type error, but that would require a more sophisticated type system.  We chose not to implement that to manage the complexity of what is primarily a pedagogical project. If instead, r is allocated using `alloc` like the following:
```
main() { var r; r = alloc {g:1}; return access(r); }
access(r) { return r.f; }
```
The record expression default initializes `f` to `0` and this is the value that is accessed and returned from the call to `access` and then from `main`.  

Second, TIP allows memory allocation, yet its runtime system does not include a garbage collector.  
The TIP program [recordLeak.tip](test/system/leak/recordLeak.tip), shown below, leaks memory because the `alloc` constructor causes the record to be allocated on the heap:
```
// recordLeak.tip
foo(x,y,z){
    var rec;
    rec = alloc {l: x, m: y, n: z};
    return (*rec).m;
}

main(){
    var i, j, a;
    a = 0;
    i = 0;
    j = 0;
    while (1000000000 > i) { 
      while (1000000000 > j) { 
        a = a + foo(3,2,4);
        j = j + 1;
      }
      i = i + 1;
    }
    return 0;
}
```
This is a valid tip program which can be compiled into an executable using and observe it's memory usage using:
```
/path/to/tipc/bin/build.sh --do test/system/leak/recordLeak.tip
./recordLeak &; top
```
You can then kill top using `Ctrl+C` and then kill the ./recordleak with `fg` and `Ctrl+C`. It's important that you disable the optimizer with the `--do` flag. Otherwise, the optimizer would be smart enough to simply return the `y` parameter's value. If we remove the alloc from `foo`, as we do in [recordNoLeak.tip](test/system/leak/recordNoLeak.tip), then the record is allocated on the call stack and it is reclained when the call to `foo` returns:
```
// recordNoLeak.tip
foo(x,y,z){
    var rec;
    rec = {l: x, m: y, n: z};
    return (*rec).m;
}

main(){
    var i, j, a;
    a = 0;
    i = 0;
    j = 0;
    while (1000000000 > i) { 
      while (1000000000 > j) { 
        a = a + foo(3,2,4);
        j = j + 1;
      }
      i = i + 1;
    }
    return 0;
}
```
We can find that this program will not create a memory leak because rec will be allocated on the stack instead of the heap as the alloc would.  

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
  
### Git Resources
+ [Pro Git Book](https://git-scm.com/book/en/v2)
+ [Git For Ages 4 And Up](https://www.youtube.com/watch?v=1ffBJ4sVUb4)

### CLion Resources
* [Using Git in CLion](https://www.jetbrains.com/help/clion/using-git-integration.html)
* [Using CLion with WSL](https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-clion.html#wsl-tooclhain)
