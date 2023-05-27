#include <algorithm>
#include <cassert>
#include <optional>

#include <libhw2/Common.h>
#include <libhw2/Library.h>
#include <libhw2/User.h>

namespace libhw2 {

User User::operator+(TextId text_id) const {
  User cpy{*this};
  return cpy += text_id;
}

User& User::operator+=(TextId text_id) {
  if (m_library)
    m_library->lend_text(*this, text_id);
  return *this;
}

User User::operator-(BorrowId borrow_id) const {
  User cpy{*this};
  return cpy -= borrow_id;
}

User& User::operator-=(BorrowId borrow_id) {
  if (m_library)
    m_library->return_text(borrow_id);
  return *this;
}

User::TextStatus User::check_text(TextId text_id) const {
  if (!m_history.contains(text_id))
    return TextStatus::Unknown;
  const auto& reading_entry = m_history.at(text_id);
  if (reading_entry.active == 0)
    return TextStatus::AlreadyRead;
  return TextStatus::CurrentlyReading;
}

void User::mark_borrowed(const Borrow& borrow) {
  auto& text_entry = m_history[borrow.text_id()];
  text_entry.borrows.push_back(borrow.id());
  text_entry.last_borrow = borrow.id();
  ++text_entry.active;
  ++m_active_readings;
}

void User::mark_returned(const Borrow& borrow) {
  if (!m_history.contains(borrow.text_id()))
    return;
  auto& text_entry = m_history.at(borrow.text_id());

  // Safety: We require that the Library manages the calls of {borrow, return}_text
  // so that they are _valid_. In other words, when calling return with a certain text
  // we have already borrowed it some time ago.
  assert(!text_entry.borrows.empty());
  assert(text_entry.active > 0);

  text_entry.last_return = borrow.id();
  --text_entry.active;

  --m_active_readings;
  ++m_past_readings;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
  os << "User { .name \"" << user.name()
     << "\", .past_readings = " << user.past_readings()
     << ", .active_readings = " << user.active_readings()
     << ", .id = " << user.id() << " }";
  return os;
}

mystd::vector<TextId> User::overdue_borrows(
    const std::chrono::duration<double>& borrowing_limit) const {
  mystd::vector<TextId> overdue_texts;
  if (m_active_readings <= 0)
    return overdue_texts;
  (void)borrowing_limit;
  return overdue_texts;
}

}  // namespace libhw2
