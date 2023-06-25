#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <sstream>

#include <libhw2/Library.h>
#include <libhw2/texts/Periodical.h>

namespace libhw2 {

std::size_t Library::add_text(const Text& new_text) {
  m_texts.try_emplace(new_text.id(), new_text);
  auto& entry = m_texts.at(new_text.id());
  ++entry.available;
  return entry.available;
}

std::size_t Library::remove_text(TextId text_id) {
  if (!m_texts.contains(text_id))
    return 0;
  auto& entry = m_texts.at(text_id);
  // If it was 0 before, then we must have lended all of them so we shouldn't
  // remove this text as we don't "have it".
  if (entry.available == 0)
    return 0;

  --entry.available;
  const auto available = entry.available;
  if (entry.available == 0)
    m_texts.erase(text_id);
  return available;
}

bool Library::add_user(User& new_user) {
  const auto [_, inserted] = m_registered_users.emplace(
      new_user.id(), UserDetails{.user_obj = std::move(new_user)});
  new_user.set_library(*this);
  return inserted;
}

bool Library::remove_user(UserId user_id) {
  if (m_registered_users.contains(user_id)) {
    auto& user = m_registered_users.at(user_id);
    user.user_obj.unset_library(*this);
  }
  return m_registered_users.erase(user_id) > 0;
}

bool Library::remove_user(const mystd::string& user_name) {
  const auto it = std::find_if(std::cbegin(m_registered_users),
                               std::cend(m_registered_users),
                               [&user_name](const auto& user_pair) {
                                 const auto& [_, user] = user_pair;
                                 return user.user_obj.name() == user_name;
                               });
  if (it == std::cend(m_registered_users))
    return false;
  return remove_user(it->second.user_obj.id());
}

bool Library::check_user(UserId user_id) {
  if (!m_registered_users.contains(user_id))
    return false;
  const auto& entry = m_registered_users.at(user_id);
  if (entry.borrowed + 1 > LIMIT_ACTIVE_READINGS_PER_USER)
    return false;
  return true;
}

std::optional<BorrowId> Library::lend_text(User& user, TextId text_id) {
  if (check_text(text_id) <= 0)
    return std::nullopt;
  if (!check_user(user.id()))
    return std::nullopt;

  auto& text_entry = m_texts.at(text_id);

  --text_entry.available;
  Borrow new_borrow{text_id, user.id()};
  // text_entry.borrows_ids.push_back(new_borrow.id());
  user.mark_borrowed(new_borrow);
  m_borrows.emplace(new_borrow.id(), new_borrow);

  return new_borrow.id();
}

bool Library::return_text(BorrowId borrow_id) {
  auto& borrow = m_borrows.at(borrow_id);
  if (borrow.end().has_value())
    return false;  // Already returned.

  assert(m_texts.contains(borrow.text_id()));

  auto& text_entry = m_texts.at(borrow.text_id());
  ++text_entry.available;
  borrow.end_now();
  auto& user = m_registered_users.at(borrow.user_id()).user_obj;
  user.mark_returned(borrow);
  return true;
}

std::size_t Library::check_text(TextId text_id) const {
  if (!m_texts.contains(text_id))
    return 0;
  return m_texts.at(text_id).available;
}

mystd::vector<Text> Library::transform_texts() const {
  mystd::vector<Text> transformed_texts;
  transformed_texts.reserve(m_texts.size());
  std::transform(
      std::cbegin(m_texts), std::cend(m_texts),
      std::back_inserter(transformed_texts),
      [](const auto& original_pair) { return original_pair.second.text_obj; });
  return transformed_texts;
}

mystd::vector<User> Library::transform_users() const {
  mystd::vector<User> transformed_users;
  transformed_users.reserve(m_texts.size());
  std::transform(
      std::cbegin(m_registered_users), std::cend(m_registered_users),
      std::back_inserter(transformed_users),
      [](const auto& original_pair) { return original_pair.second.user_obj; });
  return transformed_users;
}

mystd::vector<User> Library::sort_users() const {
  auto users = transform_users();
  std::sort(std::begin(users), std::end(users));
  return users;
}

mystd::vector<Text> Library::sort_texts(
    std::function<bool(const Text&, const Text&)> text_cmp_func) const {
  auto texts = transform_texts();
  std::sort(std::begin(texts), std::end(texts), text_cmp_func);
  return texts;
}

std::stringstream Library::print_texts(PrintTextsOptions option) const {
	std::stringstream sstr;

  auto text_cmp = [](const Text& lhs, const Text& rhs) -> bool {
    const auto left = std::tie(lhs.title(), lhs.title());
    const auto right = std::tie(rhs.title(), rhs.title());
    if (left < right)
      return true;
    if (right < left)
      return false;

    const Periodical* lhs_periodical = dynamic_cast<const Periodical*>(&lhs);
    const Periodical* rhs_periodical = dynamic_cast<const Periodical*>(&rhs);
    if (lhs_periodical != nullptr && rhs_periodical != nullptr)
      return *lhs_periodical < *rhs_periodical;
    return false;
  };

  const auto sorted_texts = sort_texts(text_cmp);
  for (const auto& text : sorted_texts) {
    if (option == PrintTextsOptions::OnlyOverdue && !is_text_overdue(text.id()))
      continue;
    sstr << text;
  }

  return sstr;
}

std::stringstream Library::print_users(PrintUsersOptions option) const {
  const auto sorted_users = sort_users();
  std::stringstream sstr;
  for (const auto& user : sorted_users) {
    switch (option) {
      case PrintUsersOptions::All:
        sstr << user << '\n';
        break;
      case PrintUsersOptions::OnlyWithOverdueBorrows:
        const auto& texts_ids =
            user.overdue_borrows(Library::LIMIT_BORROWING_DURATION_PER_TEXT);
        for (const auto& text_id : texts_ids)
          sstr << text_id << '\n';
        break;
    }
  }

  return sstr;
}

std::optional<Library> Library::load_from_file(const mystd::string& filename) {
  (void)filename;
  return std::nullopt;
}

bool Library::store_to_file(const mystd::string& filename) const {
  (void)filename;
  return false;
}

bool Library::is_text_overdue(TextId text_id) const {
  if (!m_texts.contains(text_id))
    return false;
  auto& entry = m_texts.at(text_id);
  for (const auto& borrow_id : entry.borrows) {
    const auto& borrow = m_borrows.at(borrow_id);
    if (borrow.end().has_value())
      continue;
    const std::chrono::duration<double> duration =
        *borrow.end() - borrow.start();
    if (duration > LIMIT_BORROWING_DURATION_PER_TEXT)
      return true;
  }

  return false;
}

}  // namespace libhw2
