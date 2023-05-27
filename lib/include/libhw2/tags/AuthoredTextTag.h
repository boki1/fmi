#ifndef FMI_OOP_HW2_AUTHOREDTEXT_H
#define FMI_OOP_HW2_AUTHOREDTEXT_H

#include <mystd/string.h>

#include <libhw2/Library.h>
#include <libhw2/tags/TextTag.h>

namespace libhw2 {

class AuthoredTextTag : public TextTag {
private:
	mystd::string m_author;
	mystd::string m_publisher;
	Library::Genre m_genre;

public:
	AuthoredTextTag(const mystd::string& author, const mystd::string& publisher, const Library::Genre& genre)
		: TextTag {}
		, m_author { author }
		, m_publisher { publisher }
		, m_genre { genre }
	{
	}

	AuthoredTextTag(const AuthoredTextTag&) = default;
	AuthoredTextTag& operator=(const AuthoredTextTag&) = default;

public:
	[[nodiscard]] const mystd::string& author() const { return m_author; }

	[[nodiscard]] const mystd::string& publisher() const { return m_publisher; }

	[[nodiscard]] const Library::Genre& genre() const { return m_genre; }

	[[nodiscard]] std::uint16_t genre_characteristic() const { return m_genre.characteristic; }

	[[nodiscard]] std::uint16_t genre_type() const { return m_genre.type; }

	[[nodiscard]] std::uint16_t genre_target() const { return m_genre.target; }

	[[nodiscard]] std::uint16_t genre_parity() const { return m_genre.parity; }

	void set_author(const mystd::string& author) { m_author = author; }

	void set_publisher(const mystd::string& publisher) { m_publisher = publisher; }

	void set_genre(const Library::Genre& genre) { m_genre = genre; }

	void set_genre_characteristic(std::uint16_t characteristic) { m_genre.characteristic = characteristic & 0xFF; }

	void set_genre_type(std::uint16_t type) { m_genre.type = type & 0x1F; }

	void set_genre_target(std::uint16_t target) { m_genre.target = target & 0x1F; }

	void set_genre_parity(std::uint16_t parity) { m_genre.parity = parity & 0x1; }
};

}

#endif // FMI_OOP_HW2_AUTHOREDTEXT_H
