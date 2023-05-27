#ifndef FMI_OOP_HW2_AUTHOREDTEXT_H
#define FMI_OOP_HW2_AUTHOREDTEXT_H

#include <mystd/string.h>

#include <libhw2/tags/TextTag.h>
#include <libhw2/texts/Genre.h>

namespace libhw2 {

class AuthoredTextTag : public TextTag {
protected:
	mystd::string m_author;
	mystd::string m_publisher;
	Genre m_genre;

public:
	AuthoredTextTag(const mystd::string& author, const mystd::string& publisher, const Genre& genre)
		: TextTag {}
		, m_author { author }
		, m_publisher { publisher }
		, m_genre { genre }
	{
	}

	AuthoredTextTag(const AuthoredTextTag&) = default;
	AuthoredTextTag& operator=(const AuthoredTextTag&) = default;

	virtual ~AuthoredTextTag() noexcept = default;

public:
	[[nodiscard]] const mystd::string& author() const { return m_author; }

	[[nodiscard]] const mystd::string& publisher() const { return m_publisher; }

	[[nodiscard]] const Genre& genre() const { return m_genre; }

	void set_author(const mystd::string& author) { m_author = author; }

	void set_publisher(const mystd::string& publisher) { m_publisher = publisher; }

	void set_genre(const Genre& genre) { m_genre = genre; }
};

}

#endif // FMI_OOP_HW2_AUTHOREDTEXT_H
