add_library(cfa)
add_compile_options(-Wall -Wextra -pedantic)
target_sources(
  cfa
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CubicSolver.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/CubicSolver.h
         ${CMAKE_CURRENT_SOURCE_DIR}/CFAnalyzer.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/CFAnalyzer.h
         ${CMAKE_CURRENT_SOURCE_DIR}/CallGraphBuilder.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/CallGraphBuilder.h
         ${CMAKE_CURRENT_SOURCE_DIR}/CallGraph.h
         ${CMAKE_CURRENT_SOURCE_DIR}/CallGraph.cpp)
target_include_directories(
  cfa
  PUBLIC ${CMAKE_SOURCE_DIR}/src
         ${CMAKE_SOURCE_DIR}/src/error
         ${CMAKE_SOURCE_DIR}/src/frontend/ast
         ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
         ${CMAKE_SOURCE_DIR}/src/semantic/symboltable)
target_link_libraries(cfa coverage_config loguru)
