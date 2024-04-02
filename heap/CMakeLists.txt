project(heap)

add_executable(heap heap.c test.c)
target_include_directories(heap PRIVATE .)
