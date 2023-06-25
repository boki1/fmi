#ifndef FMI_OOP_HW2_COMICSBUILDER_H
#define FMI_OOP_HW2_COMICSBUILDER_H

#include <memory>
#include <optional>

#include <libhw2/builders/TextBuilder.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/tags/AuthoredTextTag.h>
#include <libhw2/texts/Genre.h>
#include <libhw2/texts/Comics.h>

namespace libhw2 {

class ComicsBuilder : public TextBuilder {
private:
	std::optional<PeriodicalTextTag> m_periodical_tag;
	std::optional<AuthoredTextTag> m_authored_tag;

public:
	virtual ~ComicsBuilder() noexcept = default;

	void with_periodical_details(
		PeriodicalTextTag::Period period,
		std::uint64_t number) override
	{
		m_periodical_tag.emplace(period, number);
	}

	void with_authored_details(
		const mystd::string& author,
		const mystd::string& publisher,
		const Genre& genre) override
	{
		m_authored_tag.emplace(author, publisher, genre);
	}

	TextBuilder::ResultType build() override
	{
		if (!m_periodical_tag || !m_authored_tag)
			return std::unexpected(TextBuilder::Error{});
		return std::make_unique<Comics>(std::move(*m_text), *m_periodical_tag, *m_authored_tag);
	}
};

}

#endif // FMI_OOP_HW2_COMICSBUILDER_H
