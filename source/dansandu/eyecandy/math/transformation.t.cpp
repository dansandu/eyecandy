#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

using dansandu::eyecandy::math::closeTo;
using dansandu::eyecandy::math::lookAt;
using dansandu::eyecandy::math::pi;
using dansandu::eyecandy::math::rotationByX;
using dansandu::eyecandy::math::rotationByY;
using dansandu::eyecandy::math::rotationByZ;
using dansandu::eyecandy::math::scaling;
using dansandu::eyecandy::math::translation;

TEST_CASE("Transformation") {

    SECTION("scaling") {
        auto actual = scaling<int>(2, 3, 4);
        Eigen::Matrix<int, 4, 4> expected;
        // clang-format off
        expected << 2, 0, 0, 0,
                    0, 3, 0, 0,
                    0, 0, 4, 0,
                    0, 0, 0, 1;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 0));
    }

    SECTION("translation") {
        auto actual = translation<int>(7, 11, 13);
        Eigen::Matrix<int, 4, 4> expected;
        // clang-format off
        expected << 1, 0, 0,  7,
                    0, 1, 0, 11,
                    0, 0, 1, 13, 
                    0, 0, 0,  1;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 0));
    }

    SECTION("rotation by X axis") {
        auto actual = rotationByX<double>(pi<double> / 2);
        Eigen::Matrix<double, 4, 4> expected;
        // clang-format off
        expected << 1.0, 0.0,  0.0, 0.0,
                    0.0, 0.0, -1.0, 0.0,
                    0.0, 1.0,  0.0, 0.0,
                    0.0, 0.0,  0.0, 1.0;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 10e-15));
    }

    SECTION("rotation by Y axis") {
        auto actual = rotationByY<double>(pi<double> / 2);
        Eigen::Matrix<double, 4, 4> expected;
        // clang-format off
        expected <<  0.0, 0.0, 1.0, 0.0, 
                     0.0, 1.0, 0.0, 0.0,
                    -1.0, 0.0, 0.0, 0.0,
                     0.0, 0.0, 0.0, 1.0;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 10e-15));
    }

    SECTION("Rotation by Z axis") {
        auto actual = rotationByZ<double>(pi<double> / 2);
        Eigen::Matrix<double, 4, 4> expected;
        // clang-format off
        expected << 0.0, -1.0, 0.0, 0.0,
                    1.0,  0.0, 0.0, 0.0,
                    0.0,  0.0, 1.0, 0.0,
                    0.0,  0.0, 0.0, 1.0;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 10e-15));
    }

    SECTION("Look at") {
        // auto actual = lookAt<double>({0.0, 50.0, 0.0, 1.0}, {0.0, 1.0, 1.0, 1.0}, {0.0, 1.0, 0.0, 1.0});
        // auto expected = Eigen::Matrix<double>{};
    }
}
