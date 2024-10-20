cmake_minimum_required(VERSION 3.10)
project(agui)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -O0)

add_subdirectory(raylib)
add_subdirectory(agui)
add_subdirectory(test)