#ifndef FMI_OOP_HW2_LIBRARY_H
#define FMI_OOP_HW2_LIBRARY_H

#include <cstdint>
#include <type_traits>

namespace libhw2 {

class Library {
public:
	using Id = std::uint64_t;
	union Genre {
		struct {
			uint16_t characteristic : 7;
			uint16_t type : 4;
			uint16_t target : 4;
			uint16_t parity : 1;
		};
		uint16_t combined;
	};

};

static_assert(sizeof(Library::Genre) <= sizeof(std::uint16_t), "Library::Genre is too big!");

template <typename T>
concept IdGenerator = requires {
    std::is_function_v<T>;
    std::is_same_v<std::invoke_result_t<T>, Library::Id>;
};


}

#endif // FMI_OOP_HW2_LIBRARY_H
