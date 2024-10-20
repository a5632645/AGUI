file(GLOB_RECURSE test_sources "*.c" "*.h")
add_library(test_lib ${test_sources})
target_link_libraries(test_lib PUBLIC agui raylib)