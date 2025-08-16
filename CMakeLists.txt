cmake_minimum_required(VERSION 3.16)

project(AoC)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,modernize-*)
set(CMAKE_BUILD_TYPE Debug)

foreach(day RANGE 1 8)
add_executable(${day} ${day}/${day}.cpp)
endforeach()
add_executable(82 8/82.cpp)
