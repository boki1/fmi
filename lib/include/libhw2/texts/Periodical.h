#ifndef FMI_OOP_HW2_PERIODICAL_H
#define FMI_OOP_HW2_PERIODICAL_H

#include <compare>

#include <libhw2/texts/Text.h>
#include <libhw2/tags/PeriodicalTextTag.h>

namespace libhw2 {

class Periodical : public Text, public PeriodicalTextTag {


// FIXME:
// friend class PeriodicalBuilder;
// private:
public:
	Periodical(Text&& text, const PeriodicalTextTag& periodical_tag)
		: Text {std::move(text)}
		, PeriodicalTextTag { periodical_tag }
	{
	}

  virtual ~Periodical() noexcept = default;

public:
	Periodical(const Periodical&) = default;
	Periodical& operator=(const Periodical&) = default;

	Periodical(Periodical&&) noexcept = default;
	Periodical& operator=(Periodical&&) noexcept = default;

	std::strong_ordering operator<=>(const Periodical &rhs) const {
		return m_number <=> rhs.m_number;
	}
};

}

#endif // FMI_OOP_HW2_PERIODICAL_H
