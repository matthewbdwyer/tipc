add_library(optimizer)
target_sources(optimizer PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer.cpp
        )
target_include_directories(optimizer PUBLIC
        )
llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(optimizer ${llvm_libs} coverage_config)
