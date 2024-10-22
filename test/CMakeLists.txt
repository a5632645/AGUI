add_executable(timer_test timer_test.c)
target_link_libraries(timer_test PRIVATE agui)

add_executable(list_view_test list_view_test.c)
target_link_libraries(list_view_test PRIVATE agui)

add_executable(stack_layout_test stack_layout_test.c)
target_link_libraries(stack_layout_test PRIVATE agui)