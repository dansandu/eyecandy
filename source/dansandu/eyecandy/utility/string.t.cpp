#include "catch/catch.hpp"
#include "dansandu/eyecandy/utility/string.hpp"

#include <vector>

using dansandu::eyecandy::utility::format;
using dansandu::eyecandy::utility::join;

TEST_CASE("string utility") {
    SECTION("join") {
        SECTION("nonempty separator and list") {
            std::vector<int> integers = {1, 2, 3, 4};
            REQUIRE(join(integers, ", ") == "1, 2, 3, 4");
        }

        SECTION("empty separator and nonempty list") {
            std::vector<int> integers = {1, 2, 3, 4};
            REQUIRE(join(integers, "") == "1234");
        }

        SECTION("nonempty separator and empty list") {
            std::vector<int> integers;
            REQUIRE(join(integers, "#") == "");
        }

        SECTION("empty separator and empty list") {
            std::vector<int> integers;
            REQUIRE(join(integers, "") == "");
        }
    }

    SECTION("format") {
        SECTION("too few arguments") { REQUIRE_THROWS_AS(format("# + # = #", 2, 3), std::invalid_argument); }

        SECTION("too many arguments") {
            REQUIRE_THROWS_AS(format("# + # = #", 2, 3, 5, 0), std::invalid_argument);
            REQUIRE_THROWS_AS(format("no formatting", 1), std::invalid_argument);
        }

        SECTION("no arguments") { REQUIRE(format("no formatting") == "no formatting"); }

        REQUIRE(format("The # jumped the # twice", "cat", "fence") == "The cat jumped the fence twice");
    }
}
