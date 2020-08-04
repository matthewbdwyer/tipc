add_library(error)
target_sources(error PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/Error.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Error.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ParseError.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SemanticError.h
        )
target_include_directories(error PUBLIC
        )
target_link_libraries(error coverage_config)
