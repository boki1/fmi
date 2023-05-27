#ifndef FMI_OOP_HW2_PERIODICALBUILDER_H
#define FMI_OOP_HW2_PERIODICALBUILDER_H

#include <memory>
#include <optional>

#include <libhw2/builders/TextBuilder.h>
#include <libhw2/tags/LibraryTextTag.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/texts/Periodical.h>

namespace libhw2 {

class PeriodicalBuilder : public TextBuilder {
private:
	std::optional<LibraryTextTag> m_library_tag;
	std::optional<PeriodicalTextTag> m_periodical_tag;

public:
	void with_library_details(
		const mystd::string& title,
		const mystd::string& abstract,
		Library::Id id,
		std::chrono::year year) override
	{
		m_library_tag.emplace(title, abstract, id, year);
	}

	virtual void with_periodical_details(
		PeriodicalTextTag::Period period,
		std::uint64_t number) override
	{
		m_periodical_tag.emplace(period, number);
	}

	TextBuildType build()
	{
		if (!m_library_tag || !m_periodical_tag)
			return std::unexpected(TextBuildError);
		return std::make_unique<Periodical>(m_library_tag, m_periodical_tag);
	}
};

}

#endif // FMI_OOP_HW2_PERIODICALBUILDER_H
