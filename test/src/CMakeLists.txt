add_executable(timer_test timer_test.c)
target_link_libraries(timer_test PRIVATE test_lib)

add_executable(list_view_test list_view_test.c)
target_link_libraries(list_view_test PRIVATE test_lib)