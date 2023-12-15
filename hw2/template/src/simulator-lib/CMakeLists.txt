# Target for the expression processing library
add_library(simulator-lib STATIC mystd/list.h)

target_sources(
	simulator-lib
	PRIVATE
		"bar-simulator.cpp"
		"bar-simulator.h"
		"student.h"
		"bar33factorial.h"
		"bar33factorial.cpp"

		# mystd
		"mystd/find.h"
		"mystd/list.h"
		"mystd/stats_allocator.h"
)
