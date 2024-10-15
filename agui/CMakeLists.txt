file(GLOB_RECURSE AGUI_SOURCES "*.c")

add_executable(agui ${AGUI_SOURCES})
target_include_directories(agui PUBLIC include)
target_link_libraries(agui PRIVATE raylib)