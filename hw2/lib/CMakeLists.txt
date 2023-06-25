add_library(libhw2
	src/Library.cpp
	src/User.cpp)
target_include_directories(libhw2 PUBLIC
	include/)
target_compile_options(libhw2 PUBLIC
	-Wall -Wextra -Werror -std=gnu++2b)
target_link_libraries(libhw2 PUBLIC
	mystd)
