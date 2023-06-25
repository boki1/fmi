cmake_minimum_required(VERSION 3.22)
project(hw2)

set(CMAKE_CXX_STANDARD 23)

function(add_task num)
    set(task_name "task${num}")
    message("-- Adding ${task_name}")
    add_executable(${task_name} ${task_name}.cpp)
    target_include_directories(${task_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#    target_compile_definitions(${task_name} PRIVATE DEBUG)
endfunction()

add_task("1")
add_task("2")
add_task("3")
add_task("4")