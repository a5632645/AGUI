cmake_minimum_required(VERSION 3.10)
project(agui)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(raylib)
add_subdirectory(agui)
add_subdirectory(test)
add_subdirectory(agui-cpp)