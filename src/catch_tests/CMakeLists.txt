add_subdirectory(helpers)
include_directories(external/catch2)

add_executable(unit_tests)
target_sources(unit_tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ParserTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinterTest.cpp
)
target_include_directories(unit_tests PUBLIC helpers)
target_link_libraries(unit_tests tipLib test_helpers)
