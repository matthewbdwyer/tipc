# Call macro to add lexer and grammar to your build dependencies.
antlr_target(TIPGrammar TIP.g4 VISITOR)

# define a property to communicate the grammar directory to tests
SET_PROPERTY(GLOBAL PROPERTY GrammarIncludeDir ${ANTLR_TIPGrammar_OUTPUT_DIR})
include_directories(${ANTLR_TIPGrammar_OUTPUT_DIR})

# list the specific LLVM libraries for this tool
llvm_map_components_to_libnames(llvm_libs Support Core Passes)

## compiler binary target
add_executable(tipc)
target_sources(tipc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tipc.cpp)

# Make the various tipc libraries available to the compiler and linker.
# NB: We setup a dummy target to ensure that building the parser and lexer is factored
# into the dependency graph. This target must be built before before tipc and the ast lib
# but after antlr4_static.
add_subdirectory(ast)
add_subdirectory(codegen)
add_subdirectory(prettyprint)
add_subdirectory(symboltable)
add_subdirectory(types)
add_library(dummy STATIC ${ANTLR_TIPGrammar_CXX_OUTPUTS})
target_link_libraries(dummy antlr4_static)
target_link_libraries(tipc antlr4_static dummy ${llvm_libs} ast codegen prettyprint symboltable types coverage_config)
target_include_directories(tipc PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ast
        ${CMAKE_CURRENT_SOURCE_DIR}/ast/treetypes
        ${CMAKE_CURRENT_SOURCE_DIR}/codegen
        ${CMAKE_CURRENT_SOURCE_DIR}/symboltable
        ${CMAKE_CURRENT_SOURCE_DIR}/prettyprint
        ${CMAKE_CURRENT_SOURCE_DIR}/types/concrete
        ${CMAKE_CURRENT_SOURCE_DIR}/types/constraints
        ${ANTLR_TIPGrammar_OUTPUT_DIR}
        )