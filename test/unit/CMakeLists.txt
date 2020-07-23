GET_PROPERTY(GrammarIncDir GLOBAL PROPERTY GrammarIncludeDir)

include_directories(${GrammarIncDir})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(external/catch2)

add_subdirectory(helpers)

add_executable(unit_tests)
target_sources(unit_tests PUBLIC
        # First test defines CATCH_CONFIG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/TIPParserTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinterTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTableTest.cpp
)
target_include_directories(unit_tests PUBLIC helpers)
target_link_libraries(unit_tests tipLib test_helpers)
