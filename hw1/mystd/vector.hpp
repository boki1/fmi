#ifndef OOPCOIN_VECTOR_H_INCLUDED
#define OOPCOIN_VECTOR_H_INCLUDED

#include <iostream>
#include <cstdint>
#include <cassert>
#include <cstring>

#include "utilities.hpp"

namespace mystd {

    template<typename T>
    class vector {
        const static std::size_t DEFAULT_CAPACITY = 1 << 3;

        inline static std::size_t nearest_cap(std::size_t cap_candidate) noexcept {
            std::size_t cap = mystd::nearest_power_of_two(cap_candidate);
            assert(cap >= cap_candidate);
            return cap;
        }

    public:
        vector() : m_buf{new T[DEFAULT_CAPACITY]} {
            if (m_buf)
                m_capacity = DEFAULT_CAPACITY;
        }

        vector(std::initializer_list<T> init) : m_size(init.size()), m_capacity(nearest_cap(init.size())) {
            m_buf = new T[m_capacity];
            std::copy(init.begin(), init.end(), m_buf);
        }

        vector(const vector& other) : m_size(other.m_size), m_capacity(other.m_capacity) {
            m_buf = new T[m_capacity];
            std::copy(other.m_buf, other.m_buf + other.m_size, m_buf);
        }

        // Beware, this creates a shallow-copy of the internal buffer!
        vector& operator=(vector other) {
            swap(*this, other);
            return *this;
        }

        ~vector() {
            delete[] m_buf;
        }

        void push_back(const T &item) {
            if (m_size == m_capacity) {
                reserve(m_capacity + m_capacity / 2);
            }
            m_buf[m_size++] = item;
        }

        void pop_back() {
            if (m_size <= 0)
                return;
            // Manually call dtor.
            m_buf[m_size - 1].~T();
            --m_size;
        }

        void erase(const T *item_ptr)
        {
            const std::size_t index = item_ptr - m_buf;
            if (index >= m_size) {
                throw std::out_of_range("mystd::vector: Index out of range");
            }

            m_buf[index].~T();

            for (std::size_t i = index + 1; i < m_size; ++i) {
                m_buf[i - 1] = m_buf[i];
            }
            --m_size;
        }

        void reserve(size_t new_cap) {
            new_cap = nearest_cap(new_cap);
            if (new_cap <= m_capacity)
                return;
            T *new_buf = new T[new_cap];
            (void) memmove(new_buf, m_buf, m_size * sizeof(T));
            delete[] m_buf;
            m_buf = new_buf;
            m_capacity = new_cap;
        }

        T& operator[](size_t index) {
            if (index >= m_size) {
                throw std::out_of_range("mystd::vector: Index out of range");
            }
            return m_buf[index];
        }

        const T& operator[](size_t index) const {
            if (index >= m_size) {
                throw std::out_of_range("mystd::vector: Index out of range");
            }
            return m_buf[index];
        }

        // I want range-based loops :D
        [[nodiscard]] T *begin() noexcept { return m_buf; }
        [[nodiscard]] const T *begin() const noexcept { return m_buf; }
        [[nodiscard]] T *end() noexcept { return m_buf + m_size; }
        [[nodiscard]] const T *end() const noexcept { return m_buf + m_size; }

        [[nodiscard]] std::size_t size() const { return m_size; }
        [[nodiscard]] std::size_t capacity() const { return m_capacity; }

        friend std::ostream &operator<<(std::ostream &os, const vector<T> &vec) {
            os << '[' << mystd::join(vec, ", ").str() << ']';
            return os;
        }

        friend std::istream &operator>>(std::istream &is, vector<T> &vec) {
            T item;
            while (is >> item) {
                vec.push_back(item);
            }
            return is;
        }

        friend void swap(vector& first, vector& second) {
            using std::swap;
            swap(first.m_buf, second.m_buf);
            swap(first.m_size, second.m_size);
            swap(first.m_capacity, second.m_capacity);
        }


    private:
        std::size_t m_size{0};
        std::size_t m_capacity{0};
        T *m_buf{nullptr};
    };
}

#endif // OOPCOIN_VECTOR_H_INCLUDED
