#ifndef FMI_OOP_HW2_BOOK_H
#define FMI_OOP_HW2_BOOK_H

#include <libhw2/texts/Text.h>
#include <libhw2/tags/AuthoredTextTag.h>

namespace libhw2 {

class Book : public Text, public AuthoredTextTag {

// FIXME:
// friend class BookBuilder;
// private:
public:
	Book(const Text&& text, const AuthoredTextTag& authored_tag)
		: Text {std::move(text)}
		, AuthoredTextTag { authored_tag }
	{
	}

	virtual ~Book() noexcept = default;

public:
	Book(const Book&) = default;
	Book& operator=(const Book&) = default;

	Book(Book&&) noexcept = default;
	Book& operator=(Book&&) noexcept = default;
};

}

#endif //FMI_OOP_HW2_BOOK_H
