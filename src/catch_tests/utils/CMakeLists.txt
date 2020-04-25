file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_executable(utils_test ${SOURCE_FILES})
target_link_libraries(utils_test tipLib testHelpersLib)
target_link_libraries(utils_test utilsLib)
