add_library(prettyprint)
target_sources(prettyprint PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinter.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinter.h)
target_include_directories(
  prettyprint PRIVATE ${CMAKE_SOURCE_DIR}/src/frontend/ast
                      ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes)
target_link_libraries(prettyprint PRIVATE coverage_config)
