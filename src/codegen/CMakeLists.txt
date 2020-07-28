add_library(codegen)
target_sources(codegen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CodeGenerator.cpp)
target_include_directories(codegen
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/ast
        ${CMAKE_SOURCE_DIR}/src/ast/treetypes
        )
llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(codegen ${llvm_libs} coverage_config)
