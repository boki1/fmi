#ifndef FMI_OOP_HW2_PERIODICALTEXT_H
#define FMI_OOP_HW2_PERIODICALTEXT_H

#include <cstdint>
#include <string_view>

#include <libhw2/tags/TextTag.h>

namespace libhw2 {

class PeriodicalTextTag : public TextTag {
public:
	enum class Period : std::uint8_t {
		Weekly,
		Monthly,
		Yearly
	};

private:
	Period m_period;
	std::uint64_t m_number;

public:
	PeriodicalTextTag(Period period, std::uint64_t number)
		: TextTag{}
		, m_period { period }
		, m_number { number }
	{
	}

	PeriodicalTextTag(const PeriodicalTextTag&) = default;
	PeriodicalTextTag& operator=(const PeriodicalTextTag&) = default;

public:
	[[nodiscard]] std::uint64_t number() const { return m_number; }
	[[nodiscard]] Period period() const { return m_period; }
	[[nodiscard]] std::string_view period_str() const
	{
		static const char* period_str_list[] = { "Weekly", "Monthly", "Yearly" };
		return period_str_list[static_cast<std::uint8_t>(m_period)];
	}

	void set_number(std::uint64_t number) { m_number = number; }

	void set_period(Period period) { m_period = period; }
};

}

#endif // FMI_OOP_HW2_PERIODICALTEXT_H
