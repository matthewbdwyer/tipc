add_library(optimizer)
target_sources(optimizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer.cpp)
target_include_directories(optimizer PRIVATE)
llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(optimizer PRIVATE ${llvm_libs} coverage_config)
