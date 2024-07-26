# Sample static library
add_library(setlib STATIC
        ScopedPtr.h)

# Used during tests:
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

target_sources(
    setlib
    PRIVATE
        "IntegerSet.cpp"
        "IntegerSet.h"
)


