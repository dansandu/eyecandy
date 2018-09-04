#include "catch/catch.hpp"
#include "dansandu/eyecandy/geometry/sphere.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/colors.hpp"
#include "dansandu/eyecandy/raster/fragment.hpp"

using dansandu::eyecandy::geometry::sphere;
using dansandu::eyecandy::math::Point;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::drawLine;
using dansandu::eyecandy::raster::drawTriangle;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;

TEST_CASE("Fragment") {
    Image actual{100, 100};
    auto whiteShader = [&actual](auto point) { actual.plot(point, Colors::white); };
    auto overlayShader = [&actual](auto point) {
        auto color = actual.color(point) == Colors::black ? Colors::red : Colors::green;
        actual.plot(point, color);
    };

    SECTION("draw line") {
        std::initializer_list<Point> circlePoints = {{90, 50}, {89, 60}, {85, 70}, {78, 78}, {70, 84}, {60, 89},
                                                     {50, 90}, {40, 89}, {30, 85}, {22, 78}, {15, 70}, {11, 60},
                                                     {10, 50}, {11, 40}, {15, 30}, {22, 22}, {30, 15}, {40, 11},
                                                     {50, 10}, {60, 11}, {70, 15}, {78, 22}, {85, 30}, {89, 40}};
        for (auto point : circlePoints)
            drawLine({50, 50}, point, whiteShader);

        auto match = actual == readBitmapFile("resource/expected_fragment_line.bmp");
        if (!match)
            writeBitmapFile("target/actual_fragment_line.bmp", actual);

        REQUIRE(match);
    }

    SECTION("draw triangle") {
        SECTION("flat top triangle with bottom vertex to the left") {
            Point a{5, 10}, b{30, 80}, c{90, 80};

            drawTriangle(a, b, c, whiteShader);
            drawLine(a, b, overlayShader);
            drawLine(a, c, overlayShader);
            drawLine(b, c, overlayShader);

            auto match = actual == readBitmapFile("resource/expected_flat_top_triangle_bottom_to_left.bmp");
            if (!match)
                writeBitmapFile("target/actual_flat_top_triangle_bottom_to_left.bmp", actual);

            REQUIRE(match);
        }

        SECTION("flat top triangle with bottom vertex in the middle") {
            Point a{70, 50}, b{30, 80}, c{90, 80};
            drawTriangle(a, b, c, whiteShader);
            drawLine(a, b, overlayShader);
            drawLine(a, c, overlayShader);
            drawLine(b, c, overlayShader);

            auto match = actual == readBitmapFile("resource/expected_flat_top_triangle_with_middle_bottom.bmp");
            if (!match)
                writeBitmapFile("target/actual_flat_top_triangle_with middle_bottom.bmp", actual);

            REQUIRE(match);
        }

        SECTION("flat top triangle with bottom vertex to the right") {
            Point a{90, 10}, b{10, 80}, c{60, 80};
            drawTriangle(a, b, c, whiteShader);
            drawLine(a, b, overlayShader);
            drawLine(a, c, overlayShader);
            drawLine(b, c, overlayShader);

            auto match = actual == readBitmapFile("resource/expected_flat_top_triangle_bottom_to_right.bmp");
            if (!match)
                writeBitmapFile("target/actual_flat_top_triangle_bottom_to_right.bmp", actual);

            REQUIRE(match);
        }

        SECTION("flat top right triangle") {
            Point a{90, 10}, b{10, 90}, c{90, 90};
            drawTriangle(a, b, c, whiteShader);
            drawLine(a, b, overlayShader);
            drawLine(a, c, overlayShader);
            drawLine(b, c, overlayShader);

            auto match = actual == readBitmapFile("resource/expected_flat_top_right_triangle.bmp");
            if (!match)
                writeBitmapFile("target/actual_flat_top_right_triangle.bmp", actual);

            REQUIRE(match);
        }
    }
}
