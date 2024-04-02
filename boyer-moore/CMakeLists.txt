project(boyer-moore)

add_executable(boyer-moore boyer-moore.c test.c)
target_include_directories(boyer-moore PRIVATE .)
