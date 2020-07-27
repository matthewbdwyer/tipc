# Call macro to add lexer and grammar to your build dependencies.
antlr_target(TIPGrammar TIP.g4 VISITOR)

# include generated files in project environment
include_directories(${ANTLR_TIPGrammar_OUTPUT_DIR})

# define a property to communicate the grammar directory to tests
SET_PROPERTY(GLOBAL PROPERTY GrammarIncludeDir ${ANTLR_TIPGrammar_OUTPUT_DIR})

# list the specific LLVM libraries for this tool
llvm_map_components_to_libnames(llvm_libs Support Core Passes)

# build the AST and type checking libraries
add_subdirectory(ast)
add_subdirectory(types)

# TIP compiler datastructures and passes
# NB: Antlr cmake files do not like modern cmake practices. As such, we must specify the outputs
# in add_library or antrl does not recognize them as being used and will not build the target.
add_library(tip_lib STATIC ${ANTLR_TIPGrammar_CXX_OUTPUTS})
target_sources(tip_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/codegen/CodeGenerator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/symboltable/SymbolTable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/symboltable/SymbolTable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/prettyprint/PrettyPrinter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/prettyprint/PrettyPrinter.cpp)
target_link_libraries(tip_lib antlr4_static ast_lib ${llvm_libs} coverage_config)

## compiler binary target
add_executable(tipc tipc.cpp)
target_link_libraries(tipc tip_lib coverage_config)
target_include_directories(tipc PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ast
        ${CMAKE_CURRENT_SOURCE_DIR}/ast/treetypes
        ${CMAKE_CURRENT_SOURCE_DIR}/codegen
        ${CMAKE_CURRENT_SOURCE_DIR}/symboltable
        ${CMAKE_CURRENT_SOURCE_DIR}/prettyprint
        ${CMAKE_CURRENT_SOURCE_DIR}/types/concrete
        ${CMAKE_CURRENT_SOURCE_DIR}/types/constraints
        )


