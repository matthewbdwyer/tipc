add_library(test_helpers)
target_sources(test_helpers
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ASTHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ASTHelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GeneralHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GeneralHelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ParserHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ParserHelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Stringifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Stringifier.h
)
target_include_directories(test_helpers
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/types/concrete
        ${CMAKE_SOURCE_DIR}/src/types/constraints
)
target_link_libraries(test_helpers tip_lib types_lib)
