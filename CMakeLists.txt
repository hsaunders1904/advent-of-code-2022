cmake_minimum_required(VERSION 3.18)

project(advent-of-code-2022 LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_LIBRARY_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(src)
add_subdirectory(src/aoc22)
add_subdirectory(test)
