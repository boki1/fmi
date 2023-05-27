#ifndef FMI_OOP_HW2_TEXT_H
#define FMI_OOP_HW2_TEXT_H

#include <chrono>

#include <mystd/string.h>

#include <libhw2/Common.h>

namespace libhw2 {

class Text {
 public:
  Text(const mystd::string& title, const mystd::string& abstract,
       std::chrono::year year)
      : m_title{title}, m_abstract{abstract}, m_year{year} {}

  Text(const Text&) = default;
  Text& operator=(const Text&) = default;

  Text(Text&&) noexcept = default;
  Text& operator=(Text&&) noexcept = default;

  virtual ~Text() noexcept = default;

  friend std::ostream& operator<<(std::ostream& os, const Text& text) {
    // FIXME: std::cout << text.year();
    os << "Text { .title = \"" << text.title() << "\", .abstract = \""
       << text.abstract() << "\", .id = " << text.id() << " }";
	return os;
  }

  [[nodiscard]] const mystd::string& title() const { return m_title; }

  [[nodiscard]] const mystd::string& abstract() const { return m_abstract; }

  [[nodiscard]] TextId id() const { return m_id; }

  [[nodiscard]] std::chrono::year year() const { return m_year; }

  void set_title(const mystd::string& title) { m_title = title; }

  void set_abstract(const mystd::string& abstract) { m_abstract = abstract; }

  void set_year(const std::chrono::year& year) { m_year = year; }

 private:
  mystd::string m_title;
  mystd::string m_abstract;
  TextId m_id;
  std::chrono::year m_year;
};

}  // namespace libhw2

#endif  //FMI_OOP_HW2_TEXT_H
