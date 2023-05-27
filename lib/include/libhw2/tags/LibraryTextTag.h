#ifndef FMI_OOP_HW2_LIBRARYTEXT_H
#define FMI_OOP_HW2_LIBRARYTEXT_H

#include <chrono>

#include <mystd/string.h>

#include <libhw2/Library.h>
#include <libhw2/tags/TextTag.h>

namespace libhw2 {

class LibraryTextTag : public TextTag {
private:
	mystd::string m_title;
	mystd::string m_abstract;
	Library::Id m_id;
	std::chrono::year m_year;

public:
	LibraryTextTag(const mystd::string& title, const mystd::string& abstract, Library::Id id, std::chrono::year year)
		: TextTag{}
		, m_title { title }
		, m_abstract { abstract }
		, m_id { id }
		, m_year { year }
	{
	}

	LibraryTextTag(const LibraryTextTag&) = default;
	LibraryTextTag& operator=(const LibraryTextTag&) = default;

public:
	[[nodiscard]] const mystd::string& title() const { return m_title; }
	[[nodiscard]] const mystd::string& abstract() const { return m_abstract; }
	[[nodiscard]] const Library::Id& id() const { return m_id; }
	[[nodiscard]] const std::chrono::year& year() const { return m_year; }

	void set_title(const mystd::string& title) { m_title = title; }
	void set_abstract(const mystd::string& abstract) { m_abstract = abstract; }
	void set_year(const std::chrono::year& year) { m_year = year; }

	// This shouldn't be changed.
	void set_id(const Library::Id&) = delete;
};

}

#endif // FMI_OOP_HW2_LIBRARYTEXT_H
