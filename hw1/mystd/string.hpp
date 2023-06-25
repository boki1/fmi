#ifndef OOPCOIN_STRING_H_INCLUDED
#define OOPCOIN_STRING_H_INCLUDED

#include <iostream>
#include <cstring>
#include <cctype>

#include "vector.hpp"

namespace mystd {

    class string {
    public:
        string() = default;

        string(const char *cstr) {
            const auto size = std::strlen(cstr);
            m_vec.reserve(size);
            for (std::size_t i = 0; i < size; ++i) {
                m_vec.push_back(cstr[i]);
            }
        }

        string(const string &) = default;

        string &operator=(string other) {
            swap(*this, other);
            return *this;
        }

        void append(char sym) {
            m_vec.push_back(sym);
        }

        char &operator[](std::size_t index) { return m_vec[index]; }

        const char &operator[](std::size_t index) const { return m_vec[index]; }

        const char *c_str() {
            if (m_vec.size() == 0 || m_vec[m_vec.size() - 1] != '\n')
                m_vec.push_back('\0');
            return &m_vec[0];
        }

        friend void swap(string &first, string &second) {
            using std::swap;
            swap(first.m_vec, second.m_vec);
        }

        [[nodiscard]] std::size_t size() const { return m_vec.size(); }

        [[nodiscard]] char* begin() noexcept { return m_vec.begin(); }
        [[nodiscard]] const char* begin() const noexcept { return m_vec.begin(); }
        [[nodiscard]] char* end() noexcept { return m_vec.end(); }
        [[nodiscard]] const char* end() const noexcept { return m_vec.end(); }

        friend bool operator==(const mystd::string& lhs, const mystd::string& rhs) {
            if (lhs.size() != rhs.size())
                return false;
            return std::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        friend std::ostream &operator<<(std::ostream &os, const mystd::string &str) {
            os << mystd::join(str, "").str();
            return os;
        }

        friend std::istream &operator>>(std::istream &is, mystd::string &str) {
            char c;
            while (is.get(c) && !std::isspace(c)) {
                str.m_vec.push_back(c);
            }
            return is;
        }

    private:
        vector<char> m_vec;
    };

}
#endif // OOPCOIN_STRING_H_INCLUDED
