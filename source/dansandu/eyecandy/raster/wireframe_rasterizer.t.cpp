#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/image.hpp"
#include "dansandu/eyecandy/raster/wireframe_rasterizer.hpp"

using dansandu::eyecandy::math::RowVector3;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::makeWireframeRasterizer;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;

inline auto REQUIRE_IMAGE(const Image& actual, const std::string& fileName) {
    auto match = actual == readBitmapFile("resource/expected_" + fileName);
    if (!match)
        writeBitmapFile("target/actual_" + fileName, actual);

    REQUIRE(match);
}

TEST_CASE("WireframeRasterizer") {
    Image actual{50, 50};
    RowVector3<double> a{{49.0, 0.0, 35.0}}, b{{0.0, 10.0, 0.85}}, c{{20.0, 49.0, 0.0}};
    auto rasterizer = makeWireframeRasterizer<double>(
        [&actual](const auto& point, const auto&) { actual.plot(point, Colors::green); });

    rasterizer(a, b, c);

    REQUIRE_IMAGE(actual, "wireframe_rasterizer.bmp");
}
