# Build the different semantic analysis passes
add_subdirectory(weeding)
add_subdirectory(symboltable)
add_subdirectory(types)

# Define a library for all semantic analyses including the underlying passes
add_library(semantic)
target_sources(semantic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/SemanticAnalysis.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SemanticAnalysis.cpp
        )
target_include_directories(semantic PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/ast
        ${CMAKE_SOURCE_DIR}/src/ast/treetypes
        ${CMAKE_CURRENT_SOURCE_DIR}/weeding
        ${CMAKE_CURRENT_SOURCE_DIR}/symboltable
        ${CMAKE_CURRENT_SOURCE_DIR}/types
        )
target_link_libraries(semantic
        weeding
        symboltable
        types
        coverage_config
        loguru
        )
