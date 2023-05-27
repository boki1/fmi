#ifndef FMI_OOP_MYSTD_VECTOR_H
#define FMI_OOP_MYSTD_VECTOR_H

#define FMI_OOP_MYSTD_DISABLE

#ifdef FMI_OOP_MYSTD_DISABLE
#include <vector>
#endif

namespace mystd {

#ifdef FMI_OOP_MYSTD_DISABLE
using std::vector;
#else
// TODO: Implement your own.
#endif

}

#endif //FMI_OOP_MYSTD_VECTOR_H
