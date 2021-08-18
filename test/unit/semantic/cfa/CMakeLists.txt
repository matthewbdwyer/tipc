add_executable(call_graph_unit_tests)
target_sources(call_graph_unit_tests
               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/CallGraphTest.cpp)
target_include_directories(
  call_graph_unit_tests
  PRIVATE ${CMAKE_SOURCE_DIR}/src/error
          ${CMAKE_SOURCE_DIR}/src/frontend/ast
          ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
          ${CMAKE_SOURCE_DIR}/src/semantic/symboltable
          ${CMAKE_SOURCE_DIR}/src/semantic
          ${CMAKE_SOURCE_DIR}/src/semantic/types
          ${CMAKE_SOURCE_DIR}/src/semantic/cfa
          ${CMAKE_SOURCE_DIR}/src/semantic/types/concrete
          ${CMAKE_SOURCE_DIR}/src/semantic/types/constraints
          ${CMAKE_SOURCE_DIR}/src/semantic/types/solver
          ${CMAKE_SOURCE_DIR}/test/unit/helpers/)
target_link_libraries(
  call_graph_unit_tests
  PRIVATE antlr4_static
          ${llvm_libs}
          ast
          types
          symboltable
          frontend
          semantic
          codegen
          optimizer
          error
          test_helpers
          coverage_config
          cfa
          Catch2::Catch2WithMain)
