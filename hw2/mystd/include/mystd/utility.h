#ifndef FMI_OOP_HW2_UTILITY_H
#define FMI_OOP_HW2_UTILITY_H

#include <optional>
#include <utility>

namespace mystd {

template<typename T>
using optional_cref = std::reference_wrapper<const T>;

template<typename T>
using optional_ref = std::reference_wrapper<T>;

}

#endif //FMI_OOP_HW2_UTILITY_H
