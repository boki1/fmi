#ifndef SIMPLE_CMAKE_TEMPLATE_SCOPEDPTR_H
#define SIMPLE_CMAKE_TEMPLATE_SCOPEDPTR_H

#include <utility>

template <typename T>
class ScopedPtr final {

public:

    using pointer = T *;
    ///
    /// Special member functions
    ///

    ScopedPtr() = default;

    ScopedPtr(std::nullptr_t) noexcept
            : m_ptr{nullptr} {}

    explicit ScopedPtr(pointer ptr) noexcept
            : m_ptr{ptr} {}

    /// ScopedPtr is not copyable
    ScopedPtr(const ScopedPtr &) = delete;
    ScopedPtr &operator=(const ScopedPtr &) = delete;

    template <typename U>
    ScopedPtr(ScopedPtr<U> &&rhs)
            : m_ptr{rhs.release()} {}

    template <typename U>
    ScopedPtr& operator=(ScopedPtr<U> &&rhs) noexcept {
        if (this != &rhs)
            m_ptr = rhs.release();
        return *this;
    }

    ~ScopedPtr() noexcept {
        delete m_ptr;
    }

    // >=C++20
    // [[nodiscard]] auto operator<=>(const ScopedPtr &) const noexcept = default;

    ///
    /// Modifiers
    ///

    void swap(ScopedPtr& other) noexcept {
        T *tmp = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = tmp;
    }

    void reset(pointer new_ptr = nullptr) noexcept {
        delete m_ptr;
        m_ptr = new_ptr;
    }

    pointer release() noexcept {
        T *tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    ///
    /// Observers
    ///

    pointer get() const noexcept {
        return m_ptr;
    }

    pointer get() noexcept {
        return m_ptr;
    }

    pointer operator->() const noexcept {
        return m_ptr;
    }

    pointer operator->() noexcept {
        return m_ptr;
    }

    const T& operator*() const {
        return *m_ptr;
    }

    T& operator*() {
        return *m_ptr;
    }

    explicit operator bool() const noexcept { return m_ptr != nullptr; }

private:
    T *m_ptr{nullptr};
};

/// Helper
template <class T, class... Args>
ScopedPtr<T> make_scoped_ptr(Args &&...args) {
    return ScopedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif //SIMPLE_CMAKE_TEMPLATE_SCOPEDPTR_H
