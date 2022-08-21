add_library(iterators)
target_sources(
  iterators
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PreOrderIterator.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/PreOrderIterator.h
          ${CMAKE_CURRENT_SOURCE_DIR}/IteratorImpl.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/IteratorImpl.h
          ${CMAKE_CURRENT_SOURCE_DIR}/Iterator.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/IteratorUtils.tpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Iterator.h)
target_include_directories(iterators
                           PRIVATE ${CMAKE_SOURCE_DIR}/src/frontend/ast)

target_link_libraries(iterators PRIVATE ast coverage_config)
