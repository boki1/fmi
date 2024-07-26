# Executable target for the unit tests
add_executable(unit-tests)

target_link_libraries(
    unit-tests
    PRIVATE
        setlib
        Catch2::Catch2WithMain
)

target_sources(
    unit-tests
    PRIVATE
        "test-IntegerSet.cpp"
        "test-IntegerSetMore.cpp"
)

# Used during tests:
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# Automatically register all tests
catch_discover_tests(unit-tests)
