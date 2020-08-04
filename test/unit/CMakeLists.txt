GET_PROPERTY(GrammarIncDir GLOBAL PROPERTY GrammarIncludeDir)

include_directories(${GrammarIncDir})
include_directories(${PROJECT_SOURCE_DIR}/src/error)
include_directories(${PROJECT_SOURCE_DIR}/src/frontend/ast)
include_directories(${PROJECT_SOURCE_DIR}/src/frontend/ast/treetypes)
include_directories(${PROJECT_SOURCE_DIR}/src/frontend/prettyprint)
include_directories(${PROJECT_SOURCE_DIR}/src/semantic/symboltable)
include_directories(${PROJECT_SOURCE_DIR}/src/semantic/weeding)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/helpers)
include_directories(external/catch2)

add_subdirectory(helpers)
add_subdirectory(types)

add_executable(unit_tests)
target_sources(unit_tests PUBLIC
        # First test defines CATCH_CONFIG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/TIPParserTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinterTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTableTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CheckAssignableTest.cpp
)
target_include_directories(unit_tests PUBLIC helpers)
target_link_libraries(unit_tests antlr4_static ${llvm_libs} error frontend semantic codegen test_helpers)
