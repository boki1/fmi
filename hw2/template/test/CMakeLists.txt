# Executable target for the unit tests
add_executable(unit-tests test-simulation-additional.cpp)

target_link_libraries(
	unit-tests
	PRIVATE
		simulator-lib
		Catch2::Catch2WithMain
)

target_sources(
	unit-tests
	PRIVATE
		"test-simulation.cpp"
		"test-list.cpp"
		"test-simulation-additional.cpp"
)

# Automatically register all tests
catch_discover_tests(unit-tests)
