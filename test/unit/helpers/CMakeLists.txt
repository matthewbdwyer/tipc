add_library(test_helpers ASTNodeHelpers.h)
target_sources(
  test_helpers
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ASTHelper.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/ASTHelper.h
          ${CMAKE_CURRENT_SOURCE_DIR}/GeneralHelper.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/GeneralHelper.h
          ${CMAKE_CURRENT_SOURCE_DIR}/ParserHelper.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/ParserHelper.h
          ${CMAKE_CURRENT_SOURCE_DIR}/TypeHelper.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/TypeHelper.h)
target_include_directories(
  test_helpers
  PRIVATE ${CMAKE_SOURCE_DIR}/src/frontend/ast
          ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
          ${CMAKE_SOURCE_DIR}/src/types/concrete
          ${CMAKE_SOURCE_DIR}/src/types/constraints)
target_link_libraries(test_helpers PUBLIC ast frontend semantic)