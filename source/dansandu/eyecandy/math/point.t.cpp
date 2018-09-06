#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/point.hpp"

using dansandu::eyecandy::math::Point;
using dansandu::eyecandy::math::sortPointsByYx;

TEST_CASE("Point") {
    SECTION("sort points by YX") {
        SECTION("all three points with distinct Y") {
            Point a{3, 10}, b{8, 6}, c{6, 2};
            sortPointsByYx(a, b, c);

            REQUIRE(a == Point{6, 2});
            REQUIRE(b == Point{8, 6});
            REQUIRE(c == Point{3, 10});
        }

        SECTION("two points with the same Y and different X") {
            Point a{5, 10}, b{3, 5}, c{10, 5};
            sortPointsByYx(a, b, c);

            REQUIRE(a == Point{3, 5});
            REQUIRE(b == Point{10, 5});
            REQUIRE(c == Point{5, 10});
        }

        SECTION("all three points with the same Y") {
            Point a{1, 5}, b{2, 5}, c{0, 5};
            sortPointsByYx(a, b, c);

            REQUIRE(a == Point{0, 5});
            REQUIRE(b == Point{1, 5});
            REQUIRE(c == Point{2, 5});
        }
    }
}
