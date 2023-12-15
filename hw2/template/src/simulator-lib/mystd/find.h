#pragma once

namespace mystd {

template <typename InputIt, typename Predicate>
InputIt find_if(InputIt begin, InputIt end, Predicate p) {
	for (; begin != end; ++begin)
        if (p(*begin))
            return begin;
    return end;
}

}
