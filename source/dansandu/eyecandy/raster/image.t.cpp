#include "catch/catch.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>

using Catch::Equals;
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

        SECTION("plotting within bounds") {
            image.plot(5, 5, Colors::magenta);

            REQUIRE(image.color(5, 5) == Colors::magenta);
        }

        SECTION("plotting out of bounds") {
            REQUIRE_THROWS_AS(image.plot(50, 20, Colors::magenta), std::out_of_range);
            REQUIRE_THROWS_AS(image.color(15, 10), std::out_of_range);
        }
    }

    SECTION("byte array") {
        Image image{4, 1, Colors::magenta};
        std::vector<uint8_t> actual(image.pixelArray(), image.pixelArray() + image.height() * image.width() * 4);
        std::vector<uint8_t> expected = {0xFFu, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0xFFu, 0xFFu,
                                         0xFFu, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0xFFu, 0xFFu};

        REQUIRE_THAT(actual, Equals(expected));
    }
}
