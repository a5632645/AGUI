file(GLOB_RECURSE agui_sources "*.c" "*.h")
add_executable(agui ${agui_sources})
target_link_libraries(agui raylib)
target_include_directories(agui PUBLIC include)
