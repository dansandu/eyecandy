#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

using dansandu::eyecandy::math::closeTo;
using dansandu::eyecandy::math::lookAt;
using dansandu::eyecandy::math::perspective;
using dansandu::eyecandy::math::pi;
using dansandu::eyecandy::math::rotationByX;
using dansandu::eyecandy::math::rotationByY;
using dansandu::eyecandy::math::rotationByZ;
using dansandu::eyecandy::math::scaling;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::math::viewport;

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

    SECTION("rotation by Z axis") {
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

    SECTION("look at") {
        Eigen::Matrix<double, 3, 1> eye, target, up;
        eye << 1.8, 1.3, 1.9;
        target << 0.0, 0.0, 0.0;
        up << 0.0, 1.0, 0.0;

        Eigen::Matrix<double, 4, 4> expected;
        // clang-format off
        expected <<  0.726,   0.0, -0.687,    0.0,
                    -0.305, 0.895, -0.322,    0.0,
                     0.615, 0.444,  0.650, -2.922,
                       0.0,   0.0,    0.0,    1.0;
        auto actual = lookAt<double>(eye, target, up);
        // clang-format on

        REQUIRE(closeTo(actual, expected, 10e-2));
    }

    SECTION("perspective") {
        Eigen::Matrix<double, 4, 4> actual = perspective(1.0, 2000.0, 1.92, 1.5), expected;
        // clang-format off
        expected << 0.7,  0.0,  0.0,  0.0,
                    0.0, 1.05,  0.0,  0.0,
                    0.0,  0.0, -1.0, -2.0,
                    0.0,  0.0, -1.0,  0.0;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 10e-3));
    }

    SECTION("viewport") {
        Eigen::Matrix<double, 4, 4> actual = viewport(600.0, 400.0), expected;
        // clang-format off
        expected << 300.0,   0.0, 0.0, 300.0,
                      0.0, 200.0, 0.0, 200.0,
                      0.0,   0.0, 1.0,  -1.0,
                      0.0,   0.0, 0.0,   1.0;
        // clang-format on
        REQUIRE(closeTo(actual, expected, 10e-15));
    }
}
