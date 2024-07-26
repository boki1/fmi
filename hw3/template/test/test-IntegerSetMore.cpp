#include <sstream>

#include "catch2/catch_all.hpp"
#include "IntegerSet.h"

class LargeSets {
 protected:
	IntegerSet sample1;
	IntegerSet sample2;
 public:
	LargeSets()
	{
		std::istringstream i1(
#include "long-list-of-values1.inc"
		);
		sample1.deserialize(i1);

		std::istringstream i2(
#include "long-list-of-values2.inc"
		);
		sample2.deserialize(i2);
	}
};

TEST_CASE_METHOD(LargeSets, "intersection")
{
	auto intersection = sample1.getIntersection(sample2);
	CHECK(intersection.equals(sample2));
}

TEST_CASE_METHOD(LargeSets, "union")
{
	auto unionn = sample1.getUnion(sample2);
	CHECK(unionn.equals(sample1));
}

TEST_CASE_METHOD(LargeSets, "subset")
{
	CHECK(sample2.subsetOf(sample1));
}

TEST_CASE_METHOD(LargeSets, "equals")
{
	CHECK(!sample1.equals(sample2));
}
