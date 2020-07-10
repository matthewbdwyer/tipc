include_directories(external/catch2)
include_directories(helpers)
add_subdirectory(helpers)

file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_executable(unit_tests ${SOURCE_FILES})
target_link_libraries(unit_tests tipLib testHelpersLib)
