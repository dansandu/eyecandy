#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

#include <iostream>

using dansandu::eyecandy::math::identity;
using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::math::pi;
using dansandu::eyecandy::math::rotationByX;
using dansandu::eyecandy::math::scaling;
using dansandu::eyecandy::math::translation;

TEST_CASE() {

    SECTION("identity") {
        auto actual = identity<int>(3);
        Matrix<int> expected = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

        REQUIRE(actual.closeTo(expected, 0));
    }

    SECTION("scaling") {
        auto actual = scaling<int>(2, 3);
        Matrix<int> expected = {{2, 0, 0}, {0, 3, 0}, {0, 0, 1}};

        REQUIRE(actual.closeTo(expected, 0));
    }

    SECTION("translation") {
        auto actual = translation<int>(7, 11);
        Matrix<int> expected = {{1, 0, 7}, {0, 1, 11}, {0, 0, 1}};

        REQUIRE(actual.closeTo(expected, 0));
    }

    SECTION("rotation by X axis") {
        auto actual = rotationByX<double>(pi<double> / 2);
        auto expected =
            Matrix<double>{{1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, -1.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

        REQUIRE(actual.closeTo(expected, 10e-15));
    }
}
