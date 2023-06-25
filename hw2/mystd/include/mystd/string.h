#ifndef FMI_OOP_MYSTD_STRING_H
#define FMI_OOP_MYSTD_STRING_H

// There is defined the "toggle" macro.
#include <mystd/enable.h>

#ifndef FMI_OOP_USE_MYSTD_STRING

#include <string>

namespace mystd {
using std::string;
}

#else

#include <algorithm>
#include <cstring>
#include <ostream>
#include <sstream>

#include <mystd/vector.h>

// This is a quick-and-dirty string implementation based entirely on the
// quick-and-dirty vector implementation.

namespace mystd {

class string {
 public:
  string() = default;

  string(const char* cstr) {
    const auto size = std::strlen(cstr);
    m_vec.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
      m_vec.push_back(cstr[i]);
    }
  }

  string(const string&) = default;

  string& operator=(string other) {
    swap(*this, other);
    return *this;
  }

  void append(char sym) { m_vec.push_back(sym); }

  char& operator[](std::size_t index) { return m_vec[index]; }

  const char& operator[](std::size_t index) const { return m_vec[index]; }

  const char* c_str() {
    if (m_vec.size() == 0 || m_vec[m_vec.size() - 1] != '\n')
      m_vec.push_back('\0');
    return &m_vec[0];
  }

  friend void swap(string& first, string& second) {
    using std::swap;
    swap(first.m_vec, second.m_vec);
  }

  [[nodiscard]] std::size_t size() const { return m_vec.size(); }

  [[nodiscard]] char* begin() noexcept { return m_vec.begin(); }

  [[nodiscard]] const char* begin() const noexcept { return m_vec.begin(); }

  [[nodiscard]] char* end() noexcept { return m_vec.end(); }

  [[nodiscard]] const char* end() const noexcept { return m_vec.end(); }

  bool operator==(const mystd::string& rhs) const {
    if (size() != rhs.size())
      return false;
    return std::equal(begin(), end(), rhs.begin());
  }

  bool operator<(const mystd::string& rhs) const {
    const char* a = begin();
    const char* b = rhs.begin();
    for (; a < end() && b < rhs.end() && *a == *b; ++a, ++b)
      ;
    return *b - *a;
  }

  friend std::ostream& operator<<(std::ostream& os, const mystd::string& str) {
    os << mystd::join(str, "").str();
    return os;
  }

  friend std::istream& operator>>(std::istream& is, mystd::string& str) {
    char c;
    while (is.get(c) && !std::isspace(c)) {
      str.m_vec.push_back(c);
    }
    return is;
  }

 private:
  vector<char> m_vec;
};

}  // namespace mystd

#endif  // FMI_OOP_USE_MYSTD_STRING

#endif  //FMI_OOP_MYSTD_STRING_H
