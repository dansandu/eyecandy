#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

using dansandu::eyecandy::math::identity;
using dansandu::eyecandy::math::Matrix;

TEST_CASE() {

    SECTION("identity") {
        auto actual = identity<int>(3);
        Matrix<int> expected = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

        REQUIRE(actual.closeTo(expected, 0));
    }
}