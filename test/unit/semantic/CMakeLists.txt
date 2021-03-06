add_subdirectory(types)

add_executable(semantic_unit_tests)
target_sources(
  semantic_unit_tests
  PRIVATE # First test defines CATCH_CONFIG_MAIN
          ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTableTest.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/CheckAssignableTest.cpp)
target_include_directories(
  semantic_unit_tests
  PRIVATE ${CMAKE_SOURCE_DIR}/src/error
          ${CMAKE_SOURCE_DIR}/src/frontend/ast
          ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
          ${CMAKE_SOURCE_DIR}/src/semantic
          ${CMAKE_SOURCE_DIR}/src/semantic/symboltable
          ${CMAKE_SOURCE_DIR}/src/semantic/types
          ${CMAKE_SOURCE_DIR}/src/semantic/types/concrete
          ${CMAKE_SOURCE_DIR}/src/semantic/types/constraints
          ${CMAKE_SOURCE_DIR}/src/semantic/types/solver
          ${CMAKE_SOURCE_DIR}/src/semantic/weeding
          ${CMAKE_SOURCE_DIR}/test/unit/helpers
          ${CMAKE_SOURCE_DIR}/test/unit/matchers)
target_link_libraries(
  semantic_unit_tests
  PRIVATE ast
          codegen
          symboltable
          weeding
          error
          test_helpers
          coverage_config
          Catch2::Catch2)
