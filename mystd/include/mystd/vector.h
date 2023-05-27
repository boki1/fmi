#ifndef FMI_OOP_MYSTD_VECTOR_H
#define FMI_OOP_MYSTD_VECTOR_H

#ifndef FMI_OOP_USE_MYSTD_VECTOR

#include <vector>

namespace mystd {
using std::vector;
}

#else

#include <cassert>
#include <cstdint>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <sstream>

namespace mystd {

// FIXME: Should really return std::string.
template <typename Range>
std::stringstream join(const Range& range, const char* delimiter) {
  std::stringstream oss;
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

template <typename T>
class vector {
  const static std::size_t DEFAULT_CAPACITY = 1 << 3;

  inline static std::size_t nearest_cap(std::size_t cap_candidate) noexcept {
    std::size_t cap = mystd::nearest_power_of_two(cap_candidate);
    assert(cap >= cap_candidate);
    return cap;
  }

 public:
  using value_type = T;

  vector() {
	  reserve(DEFAULT_CAPACITY);
  }

  vector(std::initializer_list<T> init)
      : m_size(init.size()), m_capacity(nearest_cap(init.size())) {
	reserve(m_capacity);
    std::copy(init.begin(), init.end(), m_buf);
  }

  vector(const vector& other)
      : m_size(other.m_size), m_capacity(other.m_capacity) {
	reserve(other.m_capacity);
    std::copy(other.m_buf, other.m_buf + other.m_size, m_buf);
  }

  // Beware, this creates a shallow-copy of the internal buffer!
  vector& operator=(vector other) {
    swap(*this, other);
    return *this;
  }

  ~vector() { delete[] m_buf; }

  void push_back(const T& item) {
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

  void erase(const T* item_ptr) {
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
	T* new_buf = static_cast<T*>(operator new(new_cap * sizeof(T)));
	for (std::size_t i = 0; i < m_size; ++i)
		new (new_buf + i) T(std::move(m_buf[i]));
	for (std::size_t i = 0; i < m_size; ++i)
		m_buf[i].~T();
	operator delete(m_buf);
    m_buf = new_buf;
    m_capacity = new_cap;
  }

  T& at(size_t index) const {
    if (index >= m_size) {
      throw std::out_of_range("mystd::vector: Index out of range");
    }
    return m_buf[index];
  }

  T& operator[](size_t index) { return at(index); }

  const T& operator[](size_t index) const {
	  return at(index);
  }

  // I want range-based loops :D
  [[nodiscard]] T* begin() noexcept { return m_buf; }

  [[nodiscard]] const T* begin() const noexcept { return m_buf; }

  [[nodiscard]] T* end() noexcept { return m_buf + m_size; }

  [[nodiscard]] const T* end() const noexcept { return m_buf + m_size; }

  [[nodiscard]] std::size_t size() const { return m_size; }

  [[nodiscard]] std::size_t capacity() const { return m_capacity; }

  [[nodiscard]] bool empty() const { return m_size == 0; }

  friend std::ostream& operator<<(std::ostream& os, const vector<T>& vec) {
    os << '[' << mystd::join(vec, ", ").str() << ']';
    return os;
  }

  friend std::istream& operator>>(std::istream& is, vector<T>& vec) {
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
  T* m_buf{nullptr};
};

}  // namespace mystd

#endif // FMI_OOP_USE_MYSTD_VECTOR

#endif  // FMI_OOP_MYSTD_VECTOR_H
