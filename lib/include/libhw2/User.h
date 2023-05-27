#ifndef FMI_OOP_HW2_USER_H
#define FMI_OOP_HW2_USER_H

#include <chrono>
#include <cstdint>
#include <variant>

#include <mystd/string.h>
#include <mystd/unordered_map.h>
#include <mystd/utility.h>
#include <mystd/vector.h>

#include <libhw2/Common.h>
#include <libhw2/Borrow.h>
#include <libhw2/texts/Text.h>

// TODO:
// Make `Identifiable` - inherit the g_id_counter with id member pattern.

namespace libhw2 {

// Forward declaration.
class Library;


class User {

 public:
  enum class TextStatus { Unknown, CurrentlyReading, AlreadyRead };

  struct TextHistory {
    std::uint32_t active;
    mystd::vector<BorrowId> borrows;
    BorrowId last_borrow;
    BorrowId last_return;
  };

 private:
  UserId m_id;
  mystd::unordered_map<TextId, TextHistory> m_history;
  std::size_t m_active_readings{};
  std::size_t m_past_readings{};
  mystd::string m_name;

  // Remark: For now each user can be registered in only one library.
  Library* m_library;

 public:
  explicit User(const mystd::string& name) : m_name(name) {}

  bool operator==(const User& rhs) const { return m_name == rhs.m_name; }

  bool operator!=(const User& rhs) const { return m_name != rhs.m_name; }

  friend bool operator<(const User& lhs, const User& rhs) {
    return lhs.m_past_readings < rhs.m_past_readings;
  }

  friend bool operator<=(const User& lhs, const User& rhs) {
    return lhs.m_past_readings <= rhs.m_past_readings;
  }

  friend bool operator>(const User& lhs, const User& rhs) {
    return lhs.m_past_readings > rhs.m_past_readings;
  }

  friend bool operator>=(const User& lhs, const User& rhs) {
    return lhs.m_past_readings >= rhs.m_past_readings;
  }

  friend std::ostream& operator<<(std::ostream& os, const User& user);

  TextStatus operator[](TextId text_id) const { return check_text(text_id); }

  User operator+(TextId text_id) const;
  User& operator+=(TextId text_id);
  User operator-(BorrowId borrow_id) const;
  User& operator-=(BorrowId borrow_id);

 public:  // Actual operations of User
  [[nodiscard]] TextStatus check_text(TextId) const;

  // These deal only with the User side of the implementation.
  // It is expected that the Library instance will take care of
  // updating its own status flags for the concrete text.
  void mark_borrowed(const Borrow& borrow);
  void mark_returned(const Borrow& borrow);

  mystd::vector<TextId> overdue_borrows(
      const std::chrono::duration<double>& borrowing_limit) const;

 public:
  [[nodiscard]] std::size_t active_readings() const {
    return m_active_readings;
  }

  [[nodiscard]] std::size_t past_readings() const {
    return m_past_readings;
  }

  [[nodiscard]] const auto& history() const { return m_history; }

  [[nodiscard]] const mystd::string& name() const { return m_name; }

  [[nodiscard]] UserId id() const { return m_id; }

  void set_name(const mystd::string& name) { m_name = name; }

  void set_library(Library& library) { m_library = &library; }

  void unset_library(const Library& library) {
    // This is unused for now. Will be when (if ever) a user may be registered
    // to multiple libraries simultaneously.
    (void)library;
    m_library = nullptr;
  }
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_USER_H
