cmake_minimum_required(VERSION 3.15)

project(tipc)

# Required packages
find_package(PkgConfig REQUIRED)
pkg_search_module(UUID REQUIRED uuid)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/CMake-codecov/cmake")

# compiler must be 17
set(CMAKE_CXX_STANDARD 17)

# compile for debug if no other type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# cleanup the code with clang-tidy
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

######## Code Coverage rules ###########
add_library(coverage_config INTERFACE)

option (CODE_COVERAGE "Enable coverage reporting." ON)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(STATUS "Building with coverage reporting enabled")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  target_link_options(coverage_config INTERFACE --coverage)
endif()

######## ANTLR4 CPP target rules ###########

cmake_policy(PUSH)
# Allow the antlr jar to be found without execute permissions. 
cmake_policy(SET CMP0109 OLD)
cmake_policy(SET CMP0114 NEW)

# required if linking to static library

# Point CMake to the Corretto install location to avoid setting the system path.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(JAVA_HOME "/usr/lib/jvm/java-11-amazon-corretto")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(JAVA_HOME "/Library/Java/JavaVirtualMachines/amazon-corretto-11.jdk/Contents/Home")
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not supported")
endif()

add_definitions(-DANTLR4CPP_STATIC)

set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR4_TAG 4.10.1)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR)
if(NOT ANTLR_FOUND)
  set(ANTLR_JAR antlr-${ANTLR4_TAG}-complete.jar)
  set(ANTLR_JAR_URL https://www.antlr.org/download/${ANTLR_JAR})
  message(STATUS "Downloading antlr jar")
  file(DOWNLOAD ${ANTLR_JAR_URL} ${CMAKE_BINARY_DIR}/include/${ANTLR_JAR})
  message(STATUS "Downloading complete")
  set(ANTLR_EXECUTABLE ${CMAKE_BINARY_DIR}/include/${ANTLR_JAR})
  find_package(ANTLR REQUIRED)
endif()

cmake_policy(POP)

######## LLVM rules for installed libraries ###########
# We want to select the latest stable release even if others are installed
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(LLVM 14.0.0 REQUIRED CONFIG)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  find_package(LLVM 14.0.0 REQUIRED CONFIG)
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not supported")
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Logging.
add_library(loguru)
target_sources(loguru PUBLIC
        ${CMAKE_SOURCE_DIR}/externals/loguru/loguru.cpp
        ${CMAKE_SOURCE_DIR}/externals/loguru/loguru.hpp
        )
find_package(Threads REQUIRED)
target_link_libraries(loguru ${CMAKE_THREAD_LIBS_INIT})
if(NOT WIN32)
  target_link_libraries(loguru dl)
endif()
add_definitions(-DLOGURU_WITH_STREAMS=1 -DLOGURU_VERBOSE_SCOPE_ENDINGS=0)
include_directories(${CMAKE_SOURCE_DIR}/externals/loguru)

######## build target including unit tests ###########
add_subdirectory(src)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3)

FetchContent_MakeAvailable(Catch2)

option (BUILD_TESTING "Build the testing tree." ON)
# Only build tests if we are the top-level project
# Allows this to be used by super projects with `add_subdirectory`
if (BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
  enable_testing()
  add_subdirectory(test)
endif()
