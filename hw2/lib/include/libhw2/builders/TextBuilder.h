#ifndef FMI_OOP_HW2_TEXTBUILDER_H
#define FMI_OOP_HW2_TEXTBUILDER_H

#include <chrono>
#include <expected>
#include <memory>
#include <optional>

#include <mystd/string.h>

#include <libhw2/texts/Genre.h>
#include <libhw2/texts/Text.h>
#include <libhw2/tags/PeriodicalTextTag.h>

namespace libhw2 {


// Remark: Why use `std::unique_ptr` - we are passing down a type which derives
// from Text and not a Text instance per se.

class TextBuilder {
 protected:
  std::optional<Text> m_text;

 public:

  virtual ~TextBuilder() noexcept = default;

  virtual void with_details(const mystd::string& name,
                            const mystd::string& abstract,
                            std::chrono::year year) {
    m_text = Text{name, abstract, year};
  }

  // Remark: Do not force the implementation of functions which attach "special tags".

  virtual void with_authored_details([[maybe_unused]] const mystd::string& author,
                                     [[maybe_unused]] const mystd::string& publisher,
                                     [[maybe_unused]] const Genre& genre) {}

  virtual void with_periodical_details([[maybe_unused]] PeriodicalTextTag::Period period,
                                       [[maybe_unused]] std::uint64_t number) {}

  class Error {};
  using ResultType = std::expected<std::unique_ptr<Text>, TextBuilder::Error>;

  virtual TextBuilder::ResultType build() = 0;
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_TEXTBUILDER_H
