add_executable(frontend_unit_tests)
target_sources(
  frontend_unit_tests
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/TIPParserTest.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinterTest.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/ASTPrinterTest.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/ASTBuilderTest.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/treetypes/ASTProgramTest.cpp)
target_include_directories(
  frontend_unit_tests
  PRIVATE ${CMAKE_SOURCE_DIR}/src/error
          ${CMAKE_SOURCE_DIR}/src/frontend/
          ${CMAKE_SOURCE_DIR}/src/frontend/ast
          ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
          ${CMAKE_SOURCE_DIR}/src/frontend/prettyprint
          ${CMAKE_SOURCE_DIR}/test/unit/helpers
          ${CMAKE_SOURCE_DIR}/test/unit/matchers)
target_link_libraries(
  frontend_unit_tests
  PRIVATE antlr4_static
          ${llvm_libs}
          ast
          error
          frontend
          semantic
          codegen
          test_helpers
          coverage_config
          Catch2::Catch2WithMain)
