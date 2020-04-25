file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_library(utilsLib STATIC ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries(utilsLib tipLib)