# Target for the expression processing library
add_library(expression-lib STATIC parser.cpp parser.h operations.cpp operations.h scoped_ptr.h tokenizer.cpp tokenizer.h)

target_sources(
	expression-lib
	PRIVATE
		"expression.cpp"
		"expression.h"
)