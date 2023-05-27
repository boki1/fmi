#ifndef FMI_OOP_MYSTD_STRING_H
#define FMI_OOP_MYSTD_STRING_H

#define FMI_OOP_MYSTD_DISABLE

#ifdef FMI_OOP_MYSTD_DISABLE
#include <string>
#endif

namespace mystd {

#ifdef FMI_OOP_MYSTD_DISABLE
using std::string;
#else
// TODO: Implement your own.
#endif

}

#endif //FMI_OOP_MYSTD_STRING_H
