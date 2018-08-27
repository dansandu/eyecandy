#include "catch/catch.hpp"
#include "dansandu/eyecandy/geometry/sphere.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/colors.hpp"
#include "dansandu/eyecandy/raster/fragment.hpp"

#include <utility>

using dansandu::eyecandy::geometry::sphere;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::drawLine;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::readBitmapFile;

TEST_CASE("Fragment") {
    SECTION("draw line") {
        Image actual{100, 100};
        constexpr auto x0 = 50, y0 = 50;
        std::initializer_list<std::pair<int, int>> circlePoints = {
            {90, 50}, {89, 60}, {85, 70}, {78, 78}, {70, 84}, {60, 89}, {50, 90}, {40, 89},
            {30, 85}, {22, 78}, {15, 70}, {11, 60}, {10, 50}, {11, 40}, {15, 30}, {22, 22},
            {30, 15}, {40, 11}, {50, 10}, {60, 11}, {70, 15}, {78, 22}, {85, 30}, {89, 40}};
        for (auto point : circlePoints)
            drawLine(actual, x0, y0, point.first, point.second, Colors::white);

        REQUIRE(actual == readBitmapFile("resource/expected_line.bmp"));
    }
}
