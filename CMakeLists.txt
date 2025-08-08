cmake_minimum_required(VERSION 3.16)

project(AoC)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,modernize-*)
set(CMAKE_BUILD_TYPE Debug)

add_executable(1 1/1.cpp)
add_executable(2 2/2.cpp)
add_executable(3 3/3.cpp)
add_executable(4 4/4.cpp)
