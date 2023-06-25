#ifndef _MYSTD_H
#define _MYSTD_H

#include <cstdint> // for std::size_t only
#include <cstring>

namespace mystd {
    void memcpy(void *_dest, const void *_src, std::size_t size) {
        char *dest = static_cast<char *>(_dest);
        const char *src = static_cast<const char *>(_src);
        for (std::size_t i = 0; i < size; ++i)
            dest[i] = src[i];
    }

    void *memset(void *_dest, int value, size_t size) {
        char *dest = static_cast<char *>(_dest);
        for (std::size_t i = 0; i < size; ++i)
            dest[i] = value;
        return dest;
    }

    void assert(bool expr, const char *msg) {
#ifdef DEBUG
        if (!expr)
            std::cerr << "MYASSERT: " << msg << '\n';
#endif
    }

    struct string {
        inline static const std::size_t initial_cap = 10;

        std::size_t size{0};
        std::size_t capacity{initial_cap};
        char *ptr{new char[initial_cap]};

        string() = default;

        string(const mystd::string &) = delete;

        string(mystd::string &&) = default;

        mystd::string &operator=(const mystd::string &) = delete;

        mystd::string &operator=(mystd::string &&) = default;

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

        void insert(char sym, std::size_t idx) {
            if (size == capacity)
                resize();
            ptr[idx] = sym;
        }

        void append_str(const mystd::string &other) {
            for (std::size_t i = 0; i < other.size; ++i)
                append(other.ptr[i]);
        }

        ~string() noexcept {
            delete[] ptr;
        }

        friend std::istream &operator>>(std::istream &is, mystd::string &str) noexcept {
            std::istream::sentry sentry(is);
            if (!sentry)
                return is;
            for (int next; (next = is.get()) != is.eof() && !std::isspace(next); )
                str.append(next);
            str.append('\0');
            return is;
        }

        friend std::ostream &operator<<(std::ostream &os, const mystd::string &str) noexcept {
            os << str.ptr;
            return os;
        }

    };

    bool is_alpha(char sym) {
        return (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z');
    }

    bool is_open_paren(char sym) {
        return sym == '(';
    }

    bool is_closed_paren(char sym) {
        return sym == ')';
    }

    bool is_num(char sym) {
        return sym >= '0' && sym <= '9';
    }
}

#endif //_MYSTD_H
