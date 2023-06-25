#ifndef FMI_OOP_HW2_BORROW_H
#define FMI_OOP_HW2_BORROW_H

#include <chrono>
#include <cstdint>
#include <optional>

#include <libhw2/Common.h>
#include <libhw2/texts/Text.h>

namespace libhw2 {

class Borrow {
 public:

 private:
  BorrowId m_id;
  TextId m_text_id;
  UserId m_user_id;
  std::chrono::system_clock::time_point m_borrowing_start;
  std::optional<std::chrono::system_clock::time_point> m_borrowing_end;

 public:
  Borrow(TextId text_id, UserId user_id)
      : m_text_id{text_id},
	    m_user_id{user_id},
        m_borrowing_start{std::chrono::system_clock::now()} {}

  Borrow(const Borrow&) = default;
  Borrow& operator=(const Borrow&) = default;

  [[nodiscard]] const auto& start() const { return m_borrowing_start; }

  [[nodiscard]] const auto& end() const { return m_borrowing_end; }

  [[nodiscard]] const BorrowId& id() const { return m_id; }

  [[nodiscard]] const TextId& text_id() const { return m_text_id; }

  [[nodiscard]] const UserId& user_id() const { return m_user_id; }

  void end_now() { m_borrowing_end.emplace(std::chrono::system_clock::now()); }
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_BORROW_H
