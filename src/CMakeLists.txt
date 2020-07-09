CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

project(tipc)

# Required packages
find_package(PkgConfig REQUIRED)
pkg_search_module(UUID REQUIRED uuid)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

# compiler must be 17
set(CMAKE_CXX_STANDARD 17)

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

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR)
if(NOT ANTLR_FOUND)
  set(ANTLR_JAR antlr-4.8-complete.jar)
  set(ANTLR_JAR_URL https://www.antlr.org/download/${ANTLR_JAR})
  message(STATUS "Downloading antlr jar")
  file(DOWNLOAD ${ANTLR_JAR_URL} ${CMAKE_BINARY_DIR}/include/${ANTLR_JAR})
  message(STATUS "Downloading complete")
  list(APPEND CMAKE_PROGRAM_PATH "${CMAKE_BINARY_DIR}/include")
  find_package(ANTLR REQUIRED)
endif()

######## LLVM rules for installed libraries ###########
# We want to select the latest stable release even if others are installed
find_package(LLVM 10 REQUIRED CONFIG)

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

# TIP compiler datastructures and passes
add_library(tipLib STATIC 
            ASTBuilder.h
            ASTBuilder.cpp
            AST.h
            AST.cpp
            ASTVisitor.h
            CodeGenerator.cpp
            SymbolTable.h
            SymbolTable.cpp
            PrettyPrinter.h
            PrettyPrinter.cpp
            ${ANTLR_TIPGrammar_CXX_OUTPUTS})
target_link_libraries(tipLib antlr4_static ${llvm_libs})

# Header files in this directory used in subdirectories
include_directories(.)

## compiler binary target
add_executable(tipc tipc.cpp)
target_link_libraries(tipc tipLib)

# Testing.
add_subdirectory(catch_tests)

