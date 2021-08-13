# Build the different semantic analysis passes
add_subdirectory(weeding)
add_subdirectory(symboltable)
add_subdirectory(types)
add_subdirectory(cfa)

# Define a library for all semantic analyses including the underlying passes
add_library(semantic)
target_sources(
  semantic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SemanticAnalysis.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/SemanticAnalysis.cpp)
target_include_directories(
  semantic
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}/symboltable
          ${CMAKE_CURRENT_SOURCE_DIR}/cfa
          ${CMAKE_CURRENT_SOURCE_DIR}/types
          ${CMAKE_CURRENT_SOURCE_DIR}/types/concrete
          ${CMAKE_CURRENT_SOURCE_DIR}/types/constraints
          ${CMAKE_CURRENT_SOURCE_DIR}/types/solver
          ${CMAKE_CURRENT_SOURCE_DIR}/weeding
          ${CMAKE_SOURCE_DIR}/src/frontend/ast
          ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes)
target_link_libraries(
  semantic
  PRIVATE cfa
          ast
          weeding
          symboltable
          types
          coverage_config
          loguru)
