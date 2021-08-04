add_library(cfa)
add_compile_options(-Wall -Wextra -pedantic)
target_sources(types PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/CFACubicSolver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CFACubicSolver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CFAnalyzer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CFA.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CallGraphBuilder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CGB.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CallGraph.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CallGraph.cpp

        )
target_include_directories(types PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/error
        ${CMAKE_SOURCE_DIR}/src/frontend/ast
        ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
        )
target_link_libraries(cfa coverage_config loguru)
