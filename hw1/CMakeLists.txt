cmake_minimum_required(VERSION 3.25)
project(hw1)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(hw1
        main.cpp
        oopcoin.cpp
        cmd.cpp
        error.hpp)

target_link_libraries(hw1 mystd)

add_subdirectory(mystd)
add_subdirectory(test)