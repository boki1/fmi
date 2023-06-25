add_library(mystd SHARED
        string.hpp
        vector.hpp
        utilities.hpp
        exception.hpp)

set_target_properties(mystd PROPERTIES LINKER_LANGUAGE CXX)