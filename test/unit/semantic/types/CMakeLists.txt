add_executable(typeinference_unit_tests)
target_sources(typeinference_unit_tests
        PUBLIC
        # First test defines CATCH_CONFIG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/solvers/UnionFindTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipAlphaTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipFunctionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipIntTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRecordTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRefTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipVarTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/ConstraintGenerationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/TypeConstraintTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/solvers/UnifierTest.cpp
        # TODO
        #${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipMuTest.cpp
)
target_include_directories(typeinference_unit_tests
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/semantic/types/concrete
        ${CMAKE_SOURCE_DIR}/src/semantic/types/constraints
        ${CMAKE_SOURCE_DIR}/src/semantic/types/solver
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete
        ${CMAKE_CURRENT_SOURCE_DIR}/../helpers
)
target_link_libraries(typeinference_unit_tests antlr4_static ${llvm_libs} frontend semantic codegen optimizer error test_helpers)
