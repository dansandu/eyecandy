#include "catchorg/catch/catch.hpp"
#include "dansandu/eyecandy/transform.hpp"

using dansandu::eyecandy::Matrix4;
using dansandu::eyecandy::Vector3;
using dansandu::eyecandy::transform::lookAt;
using dansandu::eyecandy::transform::perspective;
using dansandu::eyecandy::transform::rotateByX;
using dansandu::eyecandy::transform::rotateByY;
using dansandu::eyecandy::transform::rotateByZ;
using dansandu::eyecandy::transform::scale;
using dansandu::eyecandy::transform::translate;
using dansandu::eyecandy::transform::viewport;
using dansandu::math::common::pi;
using dansandu::math::matrix::close;

TEST_CASE("transform")
{
    const auto epsilon = 1.0e-5f;

    SECTION("scale")
    {
        const auto actual = scale(2.0f, 3.0f, 4.0f);

        // clang-format off
        const auto expected = Matrix4{{{2.0f, 0.0f, 0.0f, 0.0f},
                                       {0.0f, 3.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f, 4.0f, 0.0f},
                                       {0.0f, 0.0f, 0.0f, 1.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, epsilon));
    }

    SECTION("translate")
    {
        const auto actual = translate(7.0f, 11.0f, 13.0f);

        // clang-format off
        const auto expected = Matrix4{{{1.0f,  0.0f,  0.0f,  0.0f},
                                       {0.0f,  1.0f,  0.0f,  0.0f},
                                       {0.0f,  0.0f,  1.0f,  0.0f}, 
                                       {7.0f, 11.0f, 13.0f,  1.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, epsilon));
    }

    SECTION("rotate by X axis")
    {
        const auto actual = rotateByX(pi<float> / 2.0f);

        // clang-format off
        const auto expected = Matrix4{{{1.0f,  0.0f, 0.0f, 0.0f},
                                       {0.0f,  0.0f, 1.0f, 0.0f},
                                       {0.0f, -1.0f, 0.0f, 0.0f},
                                       {0.0f,  0.0f, 0.0f, 1.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, epsilon));
    }

    SECTION("rotate by Y axis")
    {
        const auto actual = rotateByY(pi<float> / 2.0f);

        // clang-format off
        const auto expected = Matrix4{{{0.0f, 0.0f, -1.0f, 0.0f}, 
                                       {0.0f, 1.0f,  0.0f, 0.0f},
                                       {1.0f, 0.0f,  0.0f, 0.0f},
                                       {0.0f, 0.0f,  0.0f, 1.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, epsilon));
    }

    SECTION("rotate by Z axis")
    {
        const auto actual = rotateByZ(pi<float> / 2.0f);

        // clang-format off
        const auto expected = Matrix4{{{ 0.0f, 1.0f, 0.0f, 0.0f},
                                       {-1.0f, 0.0f, 0.0f, 0.0f},
                                       { 0.0f, 0.0f, 1.0f, 0.0f},
                                       { 0.0f, 0.0f, 0.0f, 1.0f}}};
        // clang-format on 

        REQUIRE(close(actual, expected, epsilon));
    }

    SECTION("look at")
    {
        const auto eye = Vector3{{2.0f, 0.0f, -5.0f}};
        const auto target = Vector3{{-50.0f, 10.0f, 2000.0f}};
        const auto up = Vector3{{0.0f, 1.0f, 0.0f}};
        
        const auto actual = lookAt(eye, target, up);

        // clang-format off
        const auto expected = Matrix4{{{-0.9996640f,  0.000129264f,  0.02592610f, 0.0f},
                                       { 0.0000000f,  0.999988000f, -0.00498579f, 0.0f},
                                       {-0.0259264f, -0.004984120f, -0.99965100f, 0.0f},
                                       { 1.8697000f, -0.025179100f, -5.05011000f, 1.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, epsilon));
    }

    SECTION("perspective")
    {
        const auto near = 1.0f;
        const auto far = 2000.0f;
        const auto fieldOfView = 1.92f;
        const auto aspect = 1.5f;

        const auto actual = perspective(near, far, fieldOfView, aspect);

        // clang-format off
        const auto expected = Matrix4{{{0.7f,  0.00f,  0.0f,  0.0f},
                                       {0.0f,  1.05f,  0.0f,  0.0f},
                                       {0.0f,  0.00f, -1.0f, -1.0f},
                                       {0.0f,  0.00f, -2.0f,  0.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, 1.0e-2f));
    }

    SECTION("viewport")
    {
        const auto width = 600.0f;
        const auto height = 400.0f;

        const auto actual = viewport(width, height);

        // clang-format off
        const auto expected = Matrix4{{{300.0f,   0.0f,  0.0f,  0.0f},
                                       {  0.0f, 200.0f,  0.0f,  0.0f},
                                       {  0.0f,   0.0f,  1.0f,  0.0f},
                                       {300.0f, 200.0f, -1.0f,  1.0f}}};
        // clang-format on

        REQUIRE(close(actual, expected, epsilon));
    }
}
