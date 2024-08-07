cmake_minimum_required(VERSION 3.20)

project(
    gtest-learning
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)
add_subdirectory(lib)

enable_testing()
add_subdirectory(test)