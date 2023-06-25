cmake_minimum_required(VERSION 3.25)
project(test2)

set(CMAKE_CXX_STANDARD 23)

add_executable(test2_task1 task1.cpp mystd.h)
target_include_directories(test2_task1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(test2_task2 task2.cpp mystd.h)
target_include_directories(test2_task2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(test2_task3 task3.cpp mystd.h)
target_include_directories(test2_task3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})