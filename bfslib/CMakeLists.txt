add_library(bfslib bfs.cpp)

target_compile_definitions(bfslib PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_link_libraries(bfslib PUBLIC parlay)
target_include_directories(bfslib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})