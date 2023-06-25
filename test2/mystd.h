#ifndef TEST2_MYSTD_H
#define TEST2_MYSTD_H

#include <cstring>

namespace mystd {

    void *memcpy(void *_dest, const void *_src, size_t num_bytes) {
        char *dest = static_cast<char *>(_dest);
        const char *src = static_cast<const char *>(_src);

        for (std::size_t i = 0; i < num_bytes; ++i)
            dest[i] = src[i];
        return dest;
    }


    struct string {
        inline static const std::size_t initial_cap = 10;

        std::size_t size{0};
        std::size_t capacity{initial_cap};
        char *ptr{new char[initial_cap]};

        string() = default;

        // Bad copy-ctor!
        string(const mystd::string &) = default;

        explicit string(const char *charp)
                : size{std::strlen(charp)} {
            ptr = new char[size + sizeof(char)];
            capacity = size;
            std::strcpy(ptr, charp);
        }

        void resize() {
            const std::size_t new_capacity = capacity * 2;
            char *new_ptr = new char[new_capacity];
            std::strcpy(new_ptr, ptr);
            delete[] ptr;
            ptr = new_ptr;
            capacity = new_capacity;
        }

        void append(char sym) {
            if (size == capacity)
                resize();
            ptr[size++] = sym;
        }

        void append_str(const mystd::string &other, bool add_space = true) {
            if (add_space)
                append(' ');
            for (std::size_t i = 0; i < other.size; ++i)
                append(other.ptr[i]);
        }

        ~string() noexcept {
            delete[] ptr;
        }

        friend std::istream &operator>>(std::istream &is, mystd::string &str) noexcept {
            char sym;
            is >> sym; // sym is '"' since data is valid according to task spec.
            while (true) {
                char peeked = is.peek();
                if (peeked == ' ') str.append(' ');
                is >> sym;
                if (sym == '"') break;
                str.append(sym);
            }

            return is;
        }

        friend std::ostream &operator<<(std::ostream &os, const mystd::string &str) noexcept {
            os << str.ptr;
            return os;
        }

        void pop_last() {
            if (size > 0)
                ptr[--size] = '\0';
        }

        bool check_substr(const mystd::string &substr) const {
            bool found = true;
            for (std::size_t i = 0; i < size; ++i) {
                if (i != 0 && ptr[i] != ' ')
                    continue;
                if (i != 0) ++i;
                for (std::size_t j = 0; j < substr.size; ++j) {
                    if (i + j >= size)
                        return false;
                    if (ptr[i + j] != substr.ptr[j]) {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return true;
                found = true;
            }

            return false;
        }

    };
}

#endif //TEST2_MYSTD_H
