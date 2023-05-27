#ifndef FMI_OOP_HW2_COMICS_H
#define FMI_OOP_HW2_COMICS_H

#include <libhw2/tags/AuthoredTextTag.h>
#include <libhw2/tags/LibraryTextTag.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/texts/Text.h>

namespace libhw2 {

class Comics : public Text, LibraryTextTag, AuthoredTextTag, PeriodicalTextTag {
public:
	Comics(const LibraryTextTag& library_tag, const PeriodicalTextTag& periodical_tag, const AuthoredTextTag& authored_tag)
		: Text {}
		, LibraryTextTag { library_tag }
		, AuthoredTextTag { authored_tag }
		, PeriodicalTextTag { periodical_tag }
	{
	}

	Comics(const Comics&) = default;
	Comics& operator=(const Comics&) = default;
};

}

#endif // FMI_OOP_HW2_COMICS_H
