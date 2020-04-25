file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_library(testHelpersLib ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(testHelpersLib tipLib)
