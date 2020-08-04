add_library(types)
target_sources(types PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipAlpha.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipAlpha.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipCons.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipCons.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipFunction.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipFunction.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipInt.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipInt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRecord.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRecord.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRef.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipRef.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipType.h
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipVar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipVar.h
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/TypeConstraint.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/TypeConstraint.h
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/TypeConstraintVisitor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints/TypeConstraintVisitor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solver/UnificationError.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solver/Unifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/solver/Unifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solver/UnionFind.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solver/UnionFind.tpp
        # TODO: Used for closing terms.
        #${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipMu.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/concrete/TipMu.h
        )
target_include_directories(types PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/error
        ${CMAKE_SOURCE_DIR}/src/frontend/ast
        ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
        ${CMAKE_SOURCE_DIR}/src/semantic/symboltable
        ${CMAKE_CURRENT_SOURCE_DIR}/concrete
        ${CMAKE_CURRENT_SOURCE_DIR}/constraints
        )

target_link_libraries(types coverage_config)
