#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

using dansandu::eyecandy::math::lookAt;
using dansandu::eyecandy::math::Matrix4;
using dansandu::eyecandy::math::perspective;
using dansandu::eyecandy::math::pi;
using dansandu::eyecandy::math::rotationByX;
using dansandu::eyecandy::math::rotationByY;
using dansandu::eyecandy::math::rotationByZ;
using dansandu::eyecandy::math::RowVector3;
using dansandu::eyecandy::math::scaling;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::math::viewport;

TEST_CASE("Transformation") {
    // clang-format off
    SECTION("scaling") {
        REQUIRE(scaling(2, 3, 4) == Matrix4<int>{{{2, 0, 0, 0},
                                                  {0, 3, 0, 0},
                                                  {0, 0, 4, 0},
                                                  {0, 0, 0, 1}}});
    }

    SECTION("translation") {
        REQUIRE(translation(7, 11, 13) == Matrix4<int>{{{1,  0,  0,  0},
                                                        {0,  1,  0,  0},
                                                        {0,  0,  1,  0}, 
                                                        {7, 11, 13,  1}}});
    }

    SECTION("rotation by X axis") {
        REQUIRE(rotationByX(pi<double> / 2.0).closeTo(Matrix4<double>{{{1.0,  0.0, 0.0, 0.0},
                                                                       {0.0,  0.0, 1.0, 0.0},
                                                                       {0.0, -1.0, 0.0, 0.0},
                                                                       {0.0,  0.0, 0.0, 1.0}}}, 10e-15));
    }

    SECTION("rotation by Y axis") {
        REQUIRE(rotationByY(pi<double> / 2.0).closeTo(Matrix4<double>{{{0.0, 0.0, -1.0, 0.0}, 
                                                                       {0.0, 1.0,  0.0, 0.0},
                                                                       {1.0, 0.0,  0.0, 0.0},
                                                                       {0.0, 0.0,  0.0, 1.0}}}, 10e-15));
    }

    SECTION("rotation by Z axis") {
        REQUIRE(rotationByZ(pi<double> / 2.0).closeTo(Matrix4<double>{{{ 0.0, 1.0, 0.0, 0.0},
                                                                       {-1.0, 0.0, 0.0, 0.0},
                                                                       { 0.0, 0.0, 1.0, 0.0},
                                                                       { 0.0, 0.0, 0.0, 1.0}}}, 10e-15));
    }

    SECTION("look at") {
        RowVector3<double> eye{{1.8, 1.3, 1.9}}, target{{0.0, 0.0, 0.0}}, up{{0.0, 1.0, 0.0}};

        REQUIRE(lookAt(eye, target, up).closeTo(Matrix4<double>{{{ 0.726, -0.305,  0.615, 0.0},
                                                                 { 0.000,  0.895,  0.444, 0.0},
                                                                 {-0.687, -0.322,  0.650, 0.0},
                                                                 { 0.000,  0.000, -2.922, 1.0}}}, 10e-2));
    }

    SECTION("perspective") {
        constexpr auto near = 1.0;
        constexpr auto far = 2000.0;
        constexpr auto fieldOfView = 1.92;
        constexpr auto aspect = 1.5;

        REQUIRE(perspective(near, far, fieldOfView, aspect).closeTo(Matrix4<double>{{{0.7,  0.0,  0.0,  0.0},
                                                                                     {0.0, 1.05,  0.0,  0.0},
                                                                                     {0.0,  0.0, -1.0, -1.0},
                                                                                     {0.0,  0.0, -2.0,  0.0}}}, 10e-3));
    }

    SECTION("viewport") {
        auto width = 600.0;
        auto height = 400.0;

        REQUIRE(viewport(width, height).closeTo(Matrix4<double>{{{300.0,   0.0,  0.0,  0.0},
                                                                 {  0.0, 200.0,  0.0,  0.0},
                                                                 {  0.0,   0.0,  1.0,  0.0},
                                                                 {300.0, 200.0, -1.0,  1.0}}}, 10e-15));
    }
    // clang-format on
}
