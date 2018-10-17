#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/image.hpp"
#include "dansandu/eyecandy/raster/line_tracer.hpp"

#include <cmath>

using dansandu::eyecandy::math::pi;
using dansandu::eyecandy::math::size_type;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::LineTracer;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;
using Point = dansandu::eyecandy::raster::LineTracer::Point;

TEST_CASE("LineTracer") {
    constexpr auto width = 50, height = 50, lineCount = 16;
    constexpr auto length = 24.0;
    Image actual{width, height};
    Point origin{{width / 2, height / 2}};
    for (auto i = 0; i < lineCount; ++i) {
        Point point;
        point.x() = std::cos(i * 2.0 * pi<double> / lineCount) * length;
        point.y() = std::sin(i * 2.0 * pi<double> / lineCount) * length;
        LineTracer lineTracer{origin, origin + point};
        do {
            auto position = lineTracer.position();
            actual.plot(position.x(), position.y(), Colors::white);
        } while (lineTracer.step());
    }

    auto match = actual == readBitmapFile("resource/expected_line_tracer.bmp");
    if (!match)
        writeBitmapFile("target/actual_line_tracer.bmp", actual);

    REQUIRE(match);
}
