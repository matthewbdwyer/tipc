add_subdirectory(types)

add_executable(semantic_unit_tests)
target_sources(semantic_unit_tests PUBLIC
        # First test defines CATCH_CONFIG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTableTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CheckAssignableTest.cpp
)
target_include_directories(semantic_unit_tests PUBLIC helpers)
target_link_libraries(semantic_unit_tests antlr4_static ${llvm_libs} error frontend semantic codegen test_helpers)
