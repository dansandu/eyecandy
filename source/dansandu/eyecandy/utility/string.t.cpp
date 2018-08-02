#include "catch/catch.hpp"
#include "dansandu/eyecandy/utility/string.hpp"

#include <vector>

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
}
