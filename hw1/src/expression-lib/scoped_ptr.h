#ifndef HOMEWORK_1_EXPRESSION_PROCESSING_SCOPED_PTR_H_INCLUDED
#define HOMEWORK_1_EXPRESSION_PROCESSING_SCOPED_PTR_H_INCLUDED

namespace mystd {

    template<typename T>
    class scoped_ptr final {

    public:

        using pointer = T *;
        ///
        /// Special member functions
        ///

        scoped_ptr() = default;

        scoped_ptr(std::nullptr_t)
        noexcept
                : m_ptr{nullptr} {}

        explicit scoped_ptr(pointer
                            ptr) noexcept
                : m_ptr{ptr} {}

        /// scoped_ptr is not copyable
        scoped_ptr(const scoped_ptr &) = delete;

        scoped_ptr &operator=(const scoped_ptr &) = delete;

        template<typename U>
        scoped_ptr(scoped_ptr<U> &&rhs)
                : m_ptr{rhs.release()} {}

        template<typename U>
        scoped_ptr &operator=(scoped_ptr<U> &&rhs) noexcept {
            if (*this != rhs)
                m_ptr = rhs.release();
            return *this;
        }

        ~scoped_ptr() noexcept {
            delete m_ptr;
        }

        [[nodiscard]] bool operator!=(const scoped_ptr &rhs) const noexcept { return m_ptr != rhs.m_ptr; }

        [[nodiscard]] bool operator==(const scoped_ptr &rhs) const noexcept { return m_ptr == rhs.m_ptr; }

        ///
        /// Modifiers
        ///

        void swap(scoped_ptr &other) noexcept {
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

        const T &operator*() const {
            return *m_ptr;
        }

        T &operator*() {
            return *m_ptr;
        }

        explicit operator bool() const noexcept { return m_ptr != nullptr; }

    private:
        T *m_ptr{nullptr};
    };

    template<class T, class... Args>
    scoped_ptr<T> make_scoped(Args &&...args) {
        return scoped_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif //HOMEWORK_1_EXPRESSION_PROCESSING_SCOPED_PTR_H_INCLUDED
