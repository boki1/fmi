#ifndef FMI_OOP_HW2_BOOKBUILDER_H
#define FMI_OOP_HW2_BOOKBUILDER_H

#include <memory>
#include <optional>

#include <libhw2/texts/Genre.h>
#include <libhw2/texts/Book.h>
#include <libhw2/builders/TextBuilder.h>
#include <libhw2/tags/AuthoredTextTag.h>

namespace libhw2 {

class BookBuilder : public TextBuilder {
private:
	std::optional<AuthoredTextTag> m_authored_tag;

public:
	virtual ~BookBuilder() noexcept = default;

	void with_authored_details(
		const mystd::string& author,
		const mystd::string& publisher,
		const Genre& genre) override
	{
		m_authored_tag.emplace(author, publisher, genre);
	}

	TextBuilder::ResultType build() override
	{
		if (!m_authored_tag)
			return std::unexpected(TextBuilder::Error{});
		return std::make_unique<Book>(std::move(*m_text), *m_authored_tag);
	}

};

}

#endif // FMI_OOP_HW2_BOOKBUILDER_H
