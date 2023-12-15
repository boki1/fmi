#pragma once

#include <utility>
#include <cstdint>

namespace mystd {

template<typename T>
class stats_allocator {
public:

	template <typename ...Args>
    T *alloc(Args&&... args) {
        ++m_allocs;
        return new T{std::forward<Args>(args)...};
    }

    void free(T *ptr) {
        ++m_frees;
        delete ptr;
    }

    [[nodiscard]] std::uint64_t allocs() const noexcept { return m_allocs; }

    [[nodiscard]] std::uint64_t frees() const noexcept { return m_frees; }

private:
    std::uint64_t m_allocs{0};
    std::uint64_t m_frees{0};
};

}
