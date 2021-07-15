#include "catchorg/catch/catch.hpp"
#include "dansandu/eyecandy/transform.hpp"

using dansandu::eyecandy::Matrix4;
using dansandu::eyecandy::Vector3;
using dansandu::eyecandy::transform::interpolate;
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
    const auto epsilon = 1.0e-3f;

    SECTION("interpolation")
    {
        const auto a = Vector3{{2.0f, -10.0f, 20.0f}};

        const auto b = Vector3{{-50.0f, 80.0f, -100.0f}};

        SECTION("line")
        {
            const auto t = 0.25f;

            const auto expected = a + (b - a) * t;

            const auto actual = interpolate(a, b, expected.x(), expected.y(), epsilon);

            REQUIRE(close(actual, expected, epsilon));
        }

        SECTION("plane")
        {
            const auto c = Vector3{{33.0f, 10.0f, -77.0f}};

            const auto u = 0.20f;

            const auto v = 0.70f;

            const auto expected = c + (a - c) * u + (b - c) * v;

            const auto actual = interpolate(a, b, c, expected.x(), expected.y(), epsilon);

            REQUIRE(close(actual, expected, epsilon));
        }
    }

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
        const auto eye = Vector3{{1.8f, 1.3f, 1.9f}};
        const auto target = Vector3{{0.0, 0.0, 0.0}};
        const auto up = Vector3{{0.0, 1.0, 0.0}};
        
        const auto actual = lookAt(eye, target, up);

        // clang-format off
        const auto expected = Matrix4{{{ 0.726f, -0.305f,  0.615f, 0.0f},
                                       { 0.000f,  0.895f,  0.444f, 0.0f},
                                       {-0.687f, -0.322f,  0.650f, 0.0f},
                                       { 0.000f,  0.000f, -2.922f, 1.0f}}};
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
        const auto expected = Matrix4{{{0.7f,  0.0f,  0.0f,  0.0f},
                                       {0.0f, 1.05f,  0.0f,  0.0f},
                                       {0.0f,  0.0f, -1.0f, -1.0f},
                                       {0.0f,  0.0f, -2.0f,  0.0f}}};
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
