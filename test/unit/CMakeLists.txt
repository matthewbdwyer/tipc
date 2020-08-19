# This property communicates the location of the ANTLR4 generated files
GET_PROPERTY(GrammarIncDir GLOBAL PROPERTY GrammarIncludeDir)
# We need to include those files in builds of the unit tests
include_directories(${GrammarIncDir})

# Unit tests need to access the development tree
include_directories(${PROJECT_SOURCE_DIR}/src/error)
include_directories(${PROJECT_SOURCE_DIR}/src/frontend/ast)
include_directories(${PROJECT_SOURCE_DIR}/src/frontend/ast/treetypes)
include_directories(${PROJECT_SOURCE_DIR}/src/frontend/prettyprint)
include_directories(${PROJECT_SOURCE_DIR}/src/semantic/symboltable)
include_directories(${PROJECT_SOURCE_DIR}/src/semantic/weeding)

# The Catch2 framework and project specific helpers are used in unit tests
include_directories(${CMAKE_SOURCE_DIR}/externals/catch2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/helpers)
add_subdirectory(helpers)

add_subdirectory(frontend)
add_subdirectory(semantic)
