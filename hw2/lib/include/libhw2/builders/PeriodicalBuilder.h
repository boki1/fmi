#ifndef FMI_OOP_HW2_PERIODICALBUILDER_H
#define FMI_OOP_HW2_PERIODICALBUILDER_H

#include <memory>
#include <optional>

#include <libhw2/builders/TextBuilder.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/texts/Periodical.h>

namespace libhw2 {

class PeriodicalBuilder : public TextBuilder {
 private:
  std::optional<PeriodicalTextTag> m_periodical_tag;

 public:
  virtual ~PeriodicalBuilder() noexcept = default;

  virtual void with_periodical_details(PeriodicalTextTag::Period period,
                                       std::uint64_t number) override {
    m_periodical_tag.emplace(period, number);
  }

  TextBuilder::ResultType build() override {
    if (!m_text || !m_periodical_tag)
      return std::unexpected(TextBuilder::Error{});
    return std::make_unique<Periodical>(std::move(*m_text), *m_periodical_tag);
  }
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_PERIODICALBUILDER_H
