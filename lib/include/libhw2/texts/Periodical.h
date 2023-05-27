#ifndef FMI_OOP_HW2_PERIODICAL_H
#define FMI_OOP_HW2_PERIODICAL_H

#include <libhw2/tags/LibraryTextTag.h>
#include <libhw2/tags/PeriodicalTextTag.h>
#include <libhw2/texts/Text.h>

namespace libhw2 {

class Periodical : public Text, LibraryTextTag, PeriodicalTextTag {
public:
	Periodical(const LibraryTextTag& library_tag, const PeriodicalTextTag& periodical_tag)
		: Text {}
		, LibraryTextTag { library_tag }
		, PeriodicalTextTag { periodical_tag }
	{
	}

	Periodical(const Periodical&) = default;
	Periodical& operator=(const Periodical&) = default;
};

}

#endif // FMI_OOP_HW2_PERIODICAL_H
