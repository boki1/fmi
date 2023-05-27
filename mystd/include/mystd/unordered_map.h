#ifndef FMI_OOP_MYSTD_MAP_H
#define FMI_OOP_MYSTD_MAP_H

// There is defined the "toggle" macro.
#include <mystd/enable.h>

#ifndef FMI_OOP_USE_MYSTD_UNORDERED_MAP

#include <unordered_map>

namespace mystd {
using std::unordered_map;
}

#else

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>

#include <mystd/vector.h>

namespace mystd {

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class unordered_map {

  struct Entry {
    Key key;
    Value value;
  };

  static constexpr std::size_t DEFAULT_CAPACITY = 10;

 public:
  unordered_map() { m_buckets.reserve(DEFAULT_CAPACITY); }

  unordered_map(const unordered_map&) = default;
  unordered_map& operator=(const unordered_map&) = default;

  ~unordered_map() noexcept = default;

  void insert(const Key& key, const Value& value) {
    const std::size_t bucket_index = m_hash(key) % m_buckets.size();
    auto& bucket = m_buckets[bucket_index];
    Entry new_entry{.key = key, .value = value};
    bucket.push_back(new_entry);
    ++m_size;
  }

 private:
  const Entry* locate(const Key& key) const {
    const std::size_t bucket_index = m_hash(key) % m_buckets.size();
    auto& bucket = m_buckets[bucket_index];
    const auto it =
        std::find_if(std::cbegin(bucket), std::cend(bucket),
                     [&](const auto& entry) { return entry.key == key; });

    if (it != std::cend(bucket))
      return it;
    return nullptr;
  }

  Entry* locate(const Key& key) {
    const std::size_t bucket_index = m_hash(key) % m_buckets.size();
    auto& bucket = m_buckets[bucket_index];
    const auto it =
        std::find_if(std::begin(bucket), std::end(bucket),
                     [&](const auto& entry) { return entry.key == key; });

    if (it != std::end(bucket))
      return it;
    return nullptr;
  }

 public:
  bool contains(const Key& key) const { return locate(key) != nullptr; }

  const Value& at(const Key& key) const {
    if (const Entry* entry = locate(key); entry != nullptr)
      return entry->value;
    throw std::out_of_range("No such key in unordered_map");
  }

  Value& at(const Key& key) {
    if (const Entry* entry = locate(key); entry != nullptr)
      return entry->value;
    throw std::out_of_range("No such key in unordered_map");
  }

  [[nodiscard]] std::size_t size() const { return m_size; }

  [[nodiscard]] bool empty() const { return m_size == 0; }

 private:
  mystd::vector<mystd::vector<Entry>> m_buckets;
  std::size_t m_size{0};
  Hash m_hash;

 public:
  class iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Entry;
    using difference_type = std::ptrdiff_t;
    using pointer = Entry*;
    using reference = Entry&;

    explicit iterator(pointer ptr) : m_current(ptr) {}

    reference operator*() const { return *m_current; }

    pointer operator->() const { return m_current; }

    iterator& operator++() {
	  inc_m_current();
      return *this;
    }

    iterator operator++(int) {
      iterator temp = *this;
	  inc_m_current();
      return temp;
    }

    friend bool operator==(const iterator& lhs, const iterator& rhs) {
      return lhs.m_current == rhs.m_current;
    }

    friend bool operator!=(const iterator& lhs, const iterator& rhs) {
      return !(lhs == rhs);
    }

   private:

    void inc_current() {
      // if (!m_current) {
      //   return;
      // }
      // auto& bucket = m_current->bucket;
      // auto& inner = bucket->second;
      // ++m_current;
      // while (m_current == inner.end() && ++bucket != m_buckets.end()) {
      //   inner = bucket->second;
      //   m_current = inner.begin();
      // }
      // if (bucket == m_buckets.end())
      //   m_current = nullptr;
    }

   private:
    pointer m_current;
	std::size_t m_bucket_index;
	std::size_t m_inner_index;
  };

  iterator begin() {
    if (m_buckets.empty()) {
      return end();
    }
    return iterator(&m_buckets[0][0]);
  }

  iterator end() {
    if (m_buckets.empty()) {
      return iterator(nullptr);
    }
    return iterator(&m_buckets.back().back() + 1);
  }

  using const_iterator = iterator;

  const_iterator cbegin() const {
    if (m_buckets.empty()) {
      return end();
    }
    // FIXME: This is not really a const iterator :D.
    return iterator(&m_buckets[0][0]);
  }

  const_iterator cend() {
    if (m_buckets.empty()) {
      return iterator(nullptr);
    }
    return iterator(&m_buckets.back().back() + 1);
  }
};

}  // namespace mystd

#endif  // FMI_OOP_USE_MYSTD_UNORDERED_MAP

#endif  //FMI_OOP_MYSTD_MAP_H
