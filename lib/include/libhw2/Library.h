#ifndef FMI_OOP_HW2_LIBRARY_H
#define FMI_OOP_HW2_LIBRARY_H

#include <cstdint>
#include <memory>
#include <type_traits>
#include <variant>
#include <functional>

#include <mystd/unordered_map.h>
#include <mystd/vector.h>

#include <libhw2/User.h>
#include <libhw2/Common.h>
#include <libhw2/texts/Text.h>

namespace libhw2 {

using namespace std::chrono_literals;

class Library {
	friend User;

 public:
  constexpr static inline std::size_t LIMIT_ACTIVE_READINGS_PER_USER = 5;

  // Duration of a month if we consider that a month has 30 days.
  constexpr static inline auto LIMIT_BORROWING_DURATION_PER_TEXT =
      24s * 60 * 60 * 30;

private:
  struct TextDetails {
    Text text_obj;
	mystd::vector<BorrowId> borrows;
	std::size_t available{0};

	explicit TextDetails(const Text& t_text_obj) : text_obj{t_text_obj} {}

	TextDetails(const TextDetails &) = default;
	TextDetails& operator=(const TextDetails &) = default;
  };

  struct UserDetails {
    User user_obj;
	std::size_t borrowed{0};
  };

 public:
  explicit Library(const mystd::string& name) : m_name{name} {}

  Library(const Library&) = default;
  Library& operator=(const Library&) = default;

 public:
  // Adds a text to the list of available ones.
  // The number of currently available texts is returned.
  std::size_t add_text(const Text& new_text);

  // Removes a text from the library. It is required that it is currently
  // available.  The number of currently available texts is returned.
  std::size_t remove_text(TextId text_id);

  bool add_user(User& new_user);
  bool remove_user(UserId user_id);
  bool remove_user(const mystd::string& user_name);
  bool check_user(UserId user_id);

  std::optional<BorrowId> lend_text(User& user, TextId text_id);
  bool return_text(BorrowId borrow_id);
  std::size_t check_text(TextId text_id) const;

  mystd::vector<Text> sort_texts(std::function<bool(const Text&, const Text&)> cmp) const;
  mystd::vector<User> sort_users() const;

  enum class PrintTextsOptions { OnlyOverdue, All };
  std::stringstream print_texts(PrintTextsOptions option = PrintTextsOptions::All) const;

  enum class PrintUsersOptions { OnlyWithOverdueBorrows, All };
  std::stringstream print_users(PrintUsersOptions option = PrintUsersOptions::All) const;

  static std::optional<Library> load_from_file(const mystd::string &filename);
  bool store_to_file(const mystd::string &filename) const;

 private:
  mystd::vector<Text> transform_texts() const;
  mystd::vector<User> transform_users() const;

  bool is_text_overdue(TextId) const;

 public:
  [[nodiscard]] const mystd::string& name() const { return m_name; }

  [[nodiscard]] const auto& registered_users() const { return m_registered_users; }

  [[nodiscard]] const auto& texts() const { return m_texts; }

  void set_name(const mystd::string& name) { m_name = name; }

 private:
  mystd::unordered_map<UserId, UserDetails> m_registered_users;
  mystd::unordered_map<TextId, TextDetails> m_texts;
  mystd::unordered_map<BorrowId, Borrow> m_borrows;
  mystd::string m_name;
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_LIBRARY_H
