add_executable(types_test)
target_sources(types_test
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipAlphaTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipFunctionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipIntTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRecordTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRefTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipVarTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/ConstraintGenerationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/TypeConstraintTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/solvers/UnifierTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/solvers/UnionFindTest.cpp
        # TODO
        #${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipMuTest.cpp
)
target_include_directories(types_test
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/types/concrete
        ${CMAKE_SOURCE_DIR}/src/types/constraints
        ${CMAKE_SOURCE_DIR}/src/types/solver
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete
        ${CMAKE_CURRENT_SOURCE_DIR}/../helpers
)
target_link_libraries(types_test antlr4_static dummy ${llvm_libs} ast codegen prettyprint symboltable types test_helpers)
