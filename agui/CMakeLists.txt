file(GLOB_RECURSE agui_sources "*.c" "*.h")
add_library(agui ${agui_sources})
target_include_directories(agui PUBLIC include)
