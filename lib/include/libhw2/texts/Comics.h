#ifndef FMI_OOP_HW2_COMICS_H
#define FMI_OOP_HW2_COMICS_H

#include <libhw2/tags/AuthoredTextTag.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/texts/Text.h>

namespace libhw2 {

class Comics : public Text, public AuthoredTextTag, public PeriodicalTextTag {

  // FIXME:
  // friend class ComicsBuilder;
  // private:
 public:
  Comics(Text&& text, const PeriodicalTextTag& periodical_tag,
         const AuthoredTextTag& authored_tag)
      : Text{std::move(text)},
        AuthoredTextTag{authored_tag},
        PeriodicalTextTag{periodical_tag} {}

  virtual ~Comics() noexcept = default;

 public:
  Comics(const Comics&) = default;
  Comics& operator=(const Comics&) = default;

  Comics(Comics&&) noexcept = default;
  Comics& operator=(Comics&&) noexcept = default;
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_COMICS_H
