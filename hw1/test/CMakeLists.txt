include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

add_executable(mystd_vector_test test_vector.cpp)
target_link_libraries(mystd_vector_test PRIVATE GTest::gtest_main mystd)
gtest_discover_tests(mystd_vector_test)
target_include_directories(mystd_vector_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

add_executable(mystd_string_test test_string.cpp)
target_link_libraries(mystd_string_test PRIVATE GTest::gtest_main mystd)
gtest_discover_tests(mystd_string_test)
target_include_directories(mystd_string_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")