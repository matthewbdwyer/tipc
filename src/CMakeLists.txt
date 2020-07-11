# Call macro to add lexer and grammar to your build dependencies.
antlr_target(TIPGrammar TIP.g4 VISITOR)

# include generated files in project environment
include_directories(${ANTLR_TIPGrammar_OUTPUT_DIR})

# define a property to communicate the grammar directory to tests
SET_PROPERTY(GLOBAL PROPERTY GrammarIncludeDir ${ANTLR_TIPGrammar_OUTPUT_DIR})

# list the specific LLVM libraries for this tool
llvm_map_components_to_libnames(llvm_libs Support Core Passes)

# TIP compiler datastructures and passes
add_library(tipLib STATIC 
            ASTBuilder.h
            ASTBuilder.cpp
            AST.h
            AST.cpp
            ASTVisitor.h
            CodeGenerator.cpp
            SymbolTable.h
            SymbolTable.cpp
            PrettyPrinter.h
            PrettyPrinter.cpp
            ${ANTLR_TIPGrammar_CXX_OUTPUTS})
target_link_libraries(tipLib antlr4_static ${llvm_libs})

# Header files in this directory used in subdirectories
include_directories(.)

## compiler binary target
add_executable(tipc tipc.cpp)
target_link_libraries(tipc tipLib)

