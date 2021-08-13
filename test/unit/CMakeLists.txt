# This property communicates the location of the ANTLR4 generated files
get_property(GrammarIncDir GLOBAL PROPERTY GrammarIncludeDir)
# We need to include those files in builds of the unit tests
include_directories(${GrammarIncDir})

add_subdirectory(helpers)
add_subdirectory(codegen)
add_subdirectory(frontend)
add_subdirectory(semantic)
