cmake_minimum_required(VERSION 4.1)
project(bfs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party/googletest)
add_subdirectory(third_party/parlay)
add_subdirectory(third_party/benchmark)
add_subdirectory(bfslib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} bfslib)

enable_testing()

add_executable(test_bfs test/test_bfs.cpp)

target_link_libraries(test_bfs PRIVATE
    bfslib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_bfs)

add_executable(bfs_benchmark benchmark/bfs_benchmark.cpp)
target_link_libraries(bfs_benchmark PRIVATE bfslib parlay benchmark::benchmark)