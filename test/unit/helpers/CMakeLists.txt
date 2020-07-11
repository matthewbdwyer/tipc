add_library(test_helpers)
target_sources(test_helpers
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ASTHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ASTHelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GeneralHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GeneralHelper.h
)
target_link_libraries(test_helpers tipLib)
