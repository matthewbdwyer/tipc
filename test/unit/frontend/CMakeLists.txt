add_executable(frontend_unit_tests)
target_sources(frontend_unit_tests PUBLIC
        # First test defines CATCH_CONFIG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/TIPParserTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinterTest.cpp
)
target_include_directories(frontend_unit_tests PUBLIC helpers)
target_link_libraries(frontend_unit_tests antlr4_static ${llvm_libs} error frontend semantic codegen test_helpers coverage_config)
