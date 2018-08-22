#include "catch/catch.hpp"
#include "dansandu/eyecandy/geometry/clipping.hpp"

using dansandu::eyecandy::geometry::clip;
using dansandu::eyecandy::geometry::clipCode;
using dansandu::eyecandy::geometry::Mesh;

static constexpr auto right = 32;
static constexpr auto left = 16;
static constexpr auto above = 8;
static constexpr auto below = 4;
static constexpr auto front = 2;
static constexpr auto behind = 1;

TEST_CASE("Clipping") {
    SECTION("clip code") {
        REQUIRE(clipCode<int>({1, 2, 0, 5}) == 0);

        REQUIRE(clipCode<int>({1, 3, 0, 2}) == above);

        REQUIRE(clipCode<int>({1, -3, 0, 2}) == below);

        REQUIRE(clipCode<int>({1, 1, 3, 2}) == front);

        REQUIRE(clipCode<int>({1, 1, -3, 2}) == behind);

        REQUIRE(clipCode<int>({3, 1, 0, 2}) == right);

        REQUIRE(clipCode<int>({-3, 1, 0, 2}) == left);

        REQUIRE(clipCode<int>({3, 4, 5, 2}) == (right | above | front));

        clip(Mesh<double>());
    }
}