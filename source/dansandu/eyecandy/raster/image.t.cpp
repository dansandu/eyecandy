#include "catch/catch.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

#include <algorithm>
#include <cstdint>

using dansandu::eyecandy::raster::Color;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;

TEST_CASE("Image") {

    SECTION("empty") {
        Image image;

        REQUIRE(image.width() == 0);
        REQUIRE(image.height() == 0);
    }

    SECTION("solid") {
        Image image{10, 20, Colors::fuchsia};

        SECTION("sanity check") {
            REQUIRE(image.width() == 10);
            REQUIRE(image.height() == 20);

            auto begin = reinterpret_cast<const uint32_t*>(image.pixelArray());
            REQUIRE(std::all_of(begin, begin + 200,
                                [](auto pixel) { return pixel == static_cast<uint32_t>(Colors::fuchsia); }));
        }

        SECTION("plotting within bounds") {
            image.plot(5, 5, Colors::magenta);

            REQUIRE(image.color(5, 5) == Colors::magenta);
        }

        SECTION("plotting out of bounds") {
            REQUIRE_THROWS_AS(image.plot(50, 20, Colors::magenta), std::out_of_range);
            REQUIRE_THROWS_AS(image.color(15, 10), std::out_of_range);
        }
    }
}