#ifndef FMI_OOP_HW2_BOOK_H
#define FMI_OOP_HW2_BOOK_H

#include <libhw2/texts/Text.h>
#include <libhw2/tags/LibraryTextTag.h>
#include <libhw2/tags/AuthoredTextTag.h>

namespace libhw2 {

class Book : public Text, LibraryTextTag, AuthoredTextTag {
public:
	Book(const LibraryTextTag& library_tag, const AuthoredTextTag& authored_tag)
		: Text {}
		, LibraryTextTag { library_tag }
		, AuthoredTextTag { authored_tag }
	{
	}

	Book(const Book&) = default;
	Book& operator=(const Book&) = default;
};

}

#endif //FMI_OOP_HW2_BOOK_H
