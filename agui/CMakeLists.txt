file(GLOB_RECURSE agui_sources "*.c" "*.h")
add_library(agui STATIC ${agui_sources})
target_include_directories(agui PUBLIC include)
target_link_libraries(agui PUBLIC raylib)