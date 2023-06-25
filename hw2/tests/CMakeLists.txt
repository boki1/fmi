include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/355d57d90d9744c41ac7c99f1e960778f1c63040.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Example test
add_executable(test_test test_test.cpp)
target_link_libraries(test_test PRIVATE gtest_main)
gtest_discover_tests(test_test)
add_test(NAME test_test COMMAND test_test)

add_executable(test_user test_user.cpp)
target_link_libraries(test_user PRIVATE gtest_main libhw2)
gtest_discover_tests(test_user)
add_test(NAME test_user COMMAND test_user)

add_executable(test_library test_library.cpp)
target_link_libraries(test_library PRIVATE gtest_main libhw2)
gtest_discover_tests(test_library)
add_test(NAME test_library COMMAND test_library)

# mystd tests

add_executable(test_string mystd/test_string.cpp)
target_link_libraries(test_string PRIVATE gtest_main mystd)
gtest_discover_tests(test_string)
add_test(NAME test_string COMMAND test_string)

add_executable(test_vector mystd/test_vector.cpp)
target_link_libraries(test_vector PRIVATE gtest_main mystd)
gtest_discover_tests(test_vector)
add_test(NAME test_vector COMMAND test_vector)

add_executable(test_unordered_map mystd/test_unordered_map.cpp)
target_link_libraries(test_unordered_map PRIVATE gtest_main mystd)
gtest_discover_tests(test_unordered_map)
add_test(NAME test_unordered_map COMMAND test_unordered_map)
