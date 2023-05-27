#ifndef FMI_OOP_HW2_BOOKBUILDER_H
#define FMI_OOP_HW2_BOOKBUILDER_H

#include <memory>
#include <optional>

#include <libhw2/builders/TextBuilder.h>
#include <libhw2/tags/LibraryTextTag.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/tags/AuthoredTextTag.h>
#include <libhw2/texts/Periodical.h>

namespace libhw2 {

class BookBuilder : public TextBuilder {
private:
	std::optional<LibraryTextTag> m_library_tag;
	std::optional<AuthoredTextTag> m_authored_tag;

public:
	void with_library_details(
		const mystd::string& title,
		const mystd::string& abstract,
		Library::Id id,
		std::chrono::year year) override
	{
		m_library_tag.emplace(title, abstract, id, year);
	}

	void with_authored_details(
		const mystd::string& author,
		const mystd::string& publisher,
		const Library::Genre& genre) override
	{
		m_authored_tag.emplace(author, publisher, genre);
	}

	TextBuildType build()
	{
		if (!m_library_tag || !m_authored_tag)
			return std::unexpected(TextBuildError);
		return std::make_unique<Book>(m_library_tag, m_authored_tag);
	}
};

}

#endif // FMI_OOP_HW2_BOOKBUILDER_H
