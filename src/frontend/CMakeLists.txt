# Call macro to add lexer and grammar to your build dependencies.
antlr_target(TIPGrammar TIP.g4 VISITOR)

# define a property to communicate the grammar directory to tests
set_property(GLOBAL PROPERTY GrammarIncludeDir ${ANTLR_TIPGrammar_OUTPUT_DIR})

# Define a target to force the compilation of the ANTLR4 generated output files.
add_library(antlrgen STATIC ${ANTLR_TIPGrammar_CXX_OUTPUTS})
target_link_libraries(antlrgen PRIVATE antlr4_static coverage_config)

# Build the different front end passes
add_subdirectory(ast)
add_subdirectory(prettyprint)

# Define a library for the front end
add_library(frontend)
target_sources(frontend PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/FrontEnd.h
                                ${CMAKE_CURRENT_SOURCE_DIR}/FrontEnd.cpp)
target_include_directories(
  frontend
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ast
          ${CMAKE_CURRENT_SOURCE_DIR}/ast/treetypes
          ${CMAKE_CURRENT_SOURCE_DIR}/prettyprint
          ${CMAKE_SOURCE_DIR}/src/error
          ${ANTLR_TIPGrammar_OUTPUT_DIR})
target_link_libraries(frontend PRIVATE antlrgen ast prettyprint coverage_config)
