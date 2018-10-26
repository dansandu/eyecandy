#include "catch/catch.hpp"

#include "dansandu/eyecandy/raster/color.hpp"

using dansandu::eyecandy::raster::Color;
using dansandu::eyecandy::raster::Colors;

TEST_CASE("Color") {
    SECTION("red channel") {
        auto color = Color{Colors::red};

        REQUIRE(color.red() == 255);
        REQUIRE(color.green() == 0);
        REQUIRE(color.blue() == 0);
        REQUIRE(color.code() == 0xFF0000FFu);
    }

    SECTION("green channel") {
        auto color = Color{Colors::green};

        REQUIRE(color.red() == 0);
        REQUIRE(color.green() == 255);
        REQUIRE(color.blue() == 0);
        REQUIRE(color.code() == 0x00FF00FFu);
    }

    SECTION("blue channel") {
        auto color = Color{Colors::blue};

        REQUIRE(color.red() == 0);
        REQUIRE(color.green() == 0);
        REQUIRE(color.blue() == 255);
        REQUIRE(color.alpha() == 255);
        REQUIRE(color.code() == 0x0000FFFFu);
    }

    SECTION("alpha channel") {
        auto color = Color{Colors::black};

        REQUIRE(color.red() == 0);
        REQUIRE(color.green() == 0);
        REQUIRE(color.blue() == 0);
        REQUIRE(color.alpha() == 255);
        REQUIRE(color.code() == 0x000000FFu);
    }

    SECTION("color from code") { REQUIRE(Colors::magenta == Color{0xFF00FFFFu}); }
}
