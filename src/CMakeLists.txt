project(tipc)

# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

# Required packages
find_package(PkgConfig REQUIRED)
pkg_search_module(UUID REQUIRED uuid)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

# compiler must be 11 
set(CMAKE_CXX_STANDARD 11)

# compile with debug flags
set(CMAKE_BUILD_TYPE Debug)

# cleanup the code with clang-tidy
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

######## ANTLR4 CPP target rules ###########
# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE /home/matthewbdwyer/lib/antlr-4.7.2-complete.jar)

# Use this setup for cs6620.cs.virginia.edu
#set(ANTLR_EXECUTABLE /sw/linux-any/antlr/4.7.2/antlr-4.7.2-complete.jar)

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

######## LLVM rules for installed libraries ###########
# We want to select the latest stable release even if others are installed
find_package(LLVM 7 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

######## Tool specific rules ###########
# Call macro to add lexer and grammar to your build dependencies.
antlr_target(TIPGrammar TIP.g4 VISITOR)

# include generated files in project environment
include_directories(${ANTLR_TIPGrammar_OUTPUT_DIR})

# list the specific LLVM libraries for this tool
llvm_map_components_to_libnames(llvm_libs Support Core Passes)

# add generated grammar to pretty printer binary target
add_executable(tipc
               tipc.cpp
               TIPtreeBuild.cpp
               TIPtreeGen.cpp
               TIPtreePrint.cpp
               TiptreeDepthFirstVisitor.cpp
               ${ANTLR_TIPGrammar_CXX_OUTPUTS})
target_link_libraries(tipc antlr4_static ${llvm_libs})
