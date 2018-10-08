#include "catch/catch.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/image.hpp"
#include "dansandu/eyecandy/raster/solid_rasterizer.hpp"
#include "dansandu/eyecandy/raster/wireframe_rasterizer.hpp"

using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::makeSolidRasterizer;
using dansandu::eyecandy::raster::makeWireframeRasterizer;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;

inline auto REQUIRE_IMAGE(const Image& actual, const std::string& fileName) {
    auto match = actual == readBitmapFile("resource/expected_" + fileName);
    if (!match)
        writeBitmapFile("target/actual_" + fileName, actual);

    REQUIRE(match);
}

TEST_CASE("SolidRasterizer") {
    using Vertex = dansandu::eyecandy::math::RowVector3<double>;

    Image actual{50, 50};
    auto solidRasterizer =
        makeSolidRasterizer<double>([&actual](const auto& point, const auto&) { actual.plot(point, Colors::white); });
    auto wireframeRasterizer = makeWireframeRasterizer<double>([&actual](const auto& point, const auto&) {
        actual.plot(point, actual.color(point) == Colors::black ? Colors::red : Colors::green);
    });

    SECTION("flat top triangle with bottom vertex to the left") {
        Vertex a{{0.0, 0.0, 0.0}}, b{{15.0, 49.0, 45.0}}, c{{49.0, 49.0, 20.0}};

        solidRasterizer(a, b, c);
        wireframeRasterizer(a, b, c);

        REQUIRE_IMAGE(actual, "flat_top_triangle_bottom_to_left.bmp");
    }

    SECTION("flat top triangle with bottom vertex to the right") {
        Vertex a{{49.0, 0.0, -15.0}}, b{{0.0, 49.0, 80.0}}, c{{15.0, 49.0, -20.0}};

        solidRasterizer(a, b, c);
        wireframeRasterizer(a, b, c);

        REQUIRE_IMAGE(actual, "flat_top_triangle_bottom_to_right.bmp");
    }

    SECTION("flat bottom tip to the left") {
        Vertex a{{0.0, 49.0, 0.0}}, b{{25.0, 0.0, 0.0}}, c{{49.0, 0.0, 0.0}};

        solidRasterizer(a, b, c);
        wireframeRasterizer(a, b, c);

        REQUIRE_IMAGE(actual, "flat_bottom_triangle_tip_to_the_left.bmp");
    }

    SECTION("flat bottom tip to the right") {
        Vertex a{{49.0, 49.0, 0.0}}, b{{0.0, 0.0, 0.0}}, c{{25.0, 0.0, 0.0}};

        solidRasterizer(a, b, c);
        wireframeRasterizer(a, b, c);

        writeBitmapFile("resource/expected_flat_bottom_triangle_tip_to_the_right.bmp", actual);

        REQUIRE_IMAGE(actual, "flat_bottom_triangle_tip_to_the_right.bmp");
    }

    SECTION("obtuse triangle") {
        Vertex a{{0.0, 49.0, 50.0}}, b{{5.0, 30.0, -20.0}}, c{{49.0, 0.0, 0.0}};

        solidRasterizer(a, b, c);
        wireframeRasterizer(a, b, c);

        REQUIRE_IMAGE(actual, "obtuse_triangle.bmp");
    }
}
