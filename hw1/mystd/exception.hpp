#include <exception>

#include "mystd/string.hpp"

#ifndef OOPCOIN_EXCEPTION_HPP_INCLUDED_
#define OOPCOIN_EXCEPTION_HPP_INCLUDED_

namespace mystd {
    class explained_exception : std::exception {
    public:
        explicit explained_exception(const mystd::string &msg)
                : m_msg{msg} {}

        explained_exception(const explained_exception &) = default;

        explained_exception &operator=(const explained_exception &) = default;

        // FIXME: Make me const
        const char *what() noexcept { return m_msg.c_str(); }

    private:
        mystd::string m_msg;
    };
}

#endif // OOPCOIN_EXCEPTION_HPP_INCLUDED_