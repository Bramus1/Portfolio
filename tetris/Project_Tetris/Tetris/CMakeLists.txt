cmake_minimum_required(VERSION 3.5)

project(Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")

add_subdirectory(src)
add_subdirectory(tests)
