#ifndef FMI_OOP_HW2_COMMON_H
#define FMI_OOP_HW2_COMMON_H

#include <cstdint>
#include <iostream>

// TODO:
// Worst file in this bunch :D.
// Use a template and the CRTP to clean this mess up (if you have time).

namespace libhw2 {

class BorrowId {
  static inline uint64_t g_id_counter{0};
  uint64_t m_id{g_id_counter++};

public:
  BorrowId() = default;
  BorrowId(const BorrowId &) = default;
  BorrowId &operator=(const BorrowId &) = default;

  friend std::ostream &operator<<(std::ostream &os, const BorrowId &id) { os << id.m_id; return os; }

  [[nodiscard]] std::uint64_t inner() const { return m_id; }

  // Impose equality and order.
  bool operator==(const BorrowId &) const = default;
  std::strong_ordering operator<=>(const BorrowId &) const = default;
};

class UserId {
  static inline uint64_t g_id_counter{0};
  uint64_t m_id{g_id_counter++};

public:
  UserId() = default;
  UserId(const UserId &) = default;
  UserId& operator=(const UserId &) = default;

  friend std::ostream &operator<<(std::ostream &os, const UserId &id) { os << id.m_id; return os; }

  [[nodiscard]] std::uint64_t inner() const { return m_id; }

  // Impose equality and order.
  bool operator==(const UserId &) const = default;
  std::strong_ordering operator<=>(const UserId &) const = default;
};

class TextId {
  static inline uint64_t g_id_counter{0};
  uint64_t m_id{g_id_counter++};

  // Forward declaration.
  friend class Text;

public:
  TextId() = default;
  TextId(const TextId &) = default;
  TextId &operator=(const TextId &) = default;

  friend std::ostream &operator<<(std::ostream &os, const TextId &id) { os << id.m_id; return os; }

  [[nodiscard]] std::uint64_t inner() const { return m_id; }

  // Impose equality and order.
  bool operator==(const TextId &) const = default;
  std::strong_ordering operator<=>(const TextId &) const = default;
};

}

namespace std {
    template<>
    struct hash<libhw2::TextId> {
       size_t operator()(const libhw2::TextId &t) const {
		   return hash<std::uint64_t>{}(t.inner());
       }
    };
 }

namespace std {
    template<>
    struct hash<libhw2::BorrowId> {
       size_t operator()(const libhw2::BorrowId &t) const {
		   return hash<std::uint64_t>{}(t.inner());
       }
    };
 }

namespace std {
    template<>
    struct hash<libhw2::UserId> {
       size_t operator()(const libhw2::UserId &t) const {
		   return hash<std::uint64_t>{}(t.inner());
       }
    };
 }

#endif // FMI_OOP_HW2_COMMON_H
