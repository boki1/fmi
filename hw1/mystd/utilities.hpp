#include <string>
#include <sstream>

#ifndef OOPCOIN_MYSTD_UTILITIES_HPP_INCLUDED
#define OOPCOIN_MYSTD_UTILITIES_HPP_INCLUDED

namespace mystd {

    // FIXME: Should really return std::string but it was said explicitly to not use it, so ...
    template<typename Range>
    std::ostringstream join(const Range &range, const char *delimiter) {
        std::ostringstream oss;
        auto it = std::begin(range);
        const auto end = std::end(range);
        if (it != end) {
            oss << *it++;
            for (; it != end; ++it) {
                oss << delimiter << *it;
            }
        }
        return oss;
    }

    // Round up to the next highest power of 2.
    // Taken from the page Bit Twiddling Hacks:
    // https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
    inline std::uint32_t nearest_power_of_two(std::uint32_t v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
    }
}

#endif // OOPCOIN_MYSTD_UTILITIES_HPP_INCLUDED