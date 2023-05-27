#ifndef FMI_OOP_HW2_TEXTBUILDER_H
#define FMI_OOP_HW2_TEXTBUILDER_H

#include <chrono>
#include <expected>
#include <memory>

#include <mystd/string.h>

#include <libhw2/Library.h>
#include <libhw2/texts/Text.h>
#include <libhw2/tags/PeriodicalTextTag.h>

namespace libhw2 {

class TextBuildError { };

using TextBuildType = std::expected<std::unique_ptr<Text>, BuildError>;

class TextBuilder {
public:
	// Remark: Do not force their implementation because not every Text has all
	// three of the tags "attached" to it.
	virtual void with_authored_details(
		const mystd::string&,
		const mystd::string&,
		const Library::Genre&) { }

	virtual void with_library_details(
		const mystd::string&,
		const mystd::string&,
		Library::Id,
		std::chrono::year) { }

	virtual void with_periodical_details(
		PeriodicalTextTag::Period,
		std::uint64_t) { }

	virtual TextBuildType build() = 0;
};

}

#endif // FMI_OOP_HW2_TEXTBUILDER_H
