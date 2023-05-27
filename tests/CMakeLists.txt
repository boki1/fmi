include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/355d57d90d9744c41ac7c99f1e960778f1c63040.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Example test
add_executable(test_test test_test.cpp)
target_link_libraries(test_test PRIVATE gtest_main libhw2)
gtest_discover_tests(test_test)
add_test(NAME test_test COMMAND test_test)
