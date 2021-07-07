add_library(symboltable)
target_sources(
  symboltable
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTable.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTable.h
          ${CMAKE_CURRENT_SOURCE_DIR}/FunctionNameCollector.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/FunctionNameCollector.h
          ${CMAKE_CURRENT_SOURCE_DIR}/LocalNameCollector.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/LocalNameCollector.h
          ${CMAKE_CURRENT_SOURCE_DIR}/FieldNameCollector.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/FieldNameCollector.h)
target_include_directories(
  symboltable
  PRIVATE ${CMAKE_SOURCE_DIR}/src/frontend/ast
          ${CMAKE_SOURCE_DIR}/src/frontend/ast/treetypes
          ${CMAKE_SOURCE_DIR}/src/error)
target_link_libraries(symboltable PRIVATE coverage_config)
