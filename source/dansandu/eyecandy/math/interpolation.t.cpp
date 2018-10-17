#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/interpolation.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <iostream>

using Catch::Detail::Approx;
using dansandu::eyecandy::math::interpolate;
using dansandu::eyecandy::math::RowVector3;

TEST_CASE("Interpolation") {
    RowVector3<double> a{{2.0, -10.0, 20.0}}, b{{-50.0, 80.0, -100.0}};

    SECTION("line") {
        auto t = 0.25;
        auto expected = a + (b - a) * t;

        REQUIRE(interpolate(a, b, expected.x(), expected.y(), 10e-5).closeTo(expected, 10e-5));
    }

    SECTION("plane") {
        RowVector3<double> c{{33.0, 10.0, -77.0}};
        auto u = 0.20, v = 0.70;
        auto expected = c + (a - c) * u + (b - c) * v;

        REQUIRE(interpolate(a, b, c, expected.x(), expected.y(), 10e-5).closeTo(expected, 10e-5));
    }
}
