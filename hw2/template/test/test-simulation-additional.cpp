#include "catch2/catch_all.hpp"
#include "simulator-lib/bar-simulator.h"

TEST_CASE("bad faculty number")
{
    std::stringstream empty{"-42 1 1\n17 0 Fraud 3"};
    std::stringstream output;
    simulate_bar(empty, output);
    REQUIRE_THROWS_AS(simulate_bar(empty, output), std::runtime_error);
}
TEST_CASE("no space in bar - with correct params")
{
    std::stringstream empty{"0 1 1\n17 0 Fraud 3"};
    std::stringstream output;
    REQUIRE_THROWS_AS(simulate_bar(empty, output), std::runtime_error);
}

TEST_CASE("Out of order group") {
    std::stringstream inp{
            "1 1 2\n"
            "100 6 Fraud 5\n"
            "200 0 Micromanagement 2\n"};
    std::ostringstream output;
    REQUIRE_THROWS_AS(simulate_bar(inp, output), std::runtime_error);
}