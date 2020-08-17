add_library(codegen)
target_sources(codegen PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/CodeGenerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CodeGenerator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CodeGenFunctions.cpp
        )
target_include_directories(codegen PUBLIC
        ${CMAKE_SOURCE_DIR}/src/frontend/ast
        ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
        ${CMAKE_SOURCE_DIR}/src/semantic
        ${CMAKE_SOURCE_DIR}/src/semantic/symboltable
        ${CMAKE_SOURCE_DIR}/src/semantic/weeding
        ${CMAKE_SOURCE_DIR}/src/semantic/types
        ${CMAKE_SOURCE_DIR}/src/semantic/types/solver
        ${CMAKE_SOURCE_DIR}/src/semantic/types/concrete
        ${CMAKE_SOURCE_DIR}/src/semantic/types/constraints
        )
llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(codegen ${llvm_libs} coverage_config)
