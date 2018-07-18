#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

using dansandu::eyecandy::math::identity;
using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::math::scaling;

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
}
