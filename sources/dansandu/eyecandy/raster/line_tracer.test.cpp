#include "catchorg/catch/catch.hpp"
#include "dansandu/canvas/bitmap.hpp"
#include "dansandu/canvas/color.hpp"
#include "dansandu/canvas/image.hpp"
#include "dansandu/eyecandy/common.hpp"
#include "dansandu/eyecandy/raster/line_tracer.hpp"
#include "dansandu/math/common.hpp"

#include <cmath>

using dansandu::canvas::bitmap::readBitmapFile;
using dansandu::canvas::bitmap::writeBitmapFile;
using dansandu::canvas::color::Colors;
using dansandu::canvas::image::Image;
using dansandu::eyecandy::Point2;
using dansandu::eyecandy::raster::line_tracer::LineTracer;
using dansandu::math::common::pi;

TEST_CASE("line_tracer")
{
    const auto width = 101;
    const auto height = 101;
    const auto lineCount = 20;
    const auto startLength = 10.0f;
    const auto endLength = 50.0f;
    const auto center = Point2{{width / 2, height / 2}};

    auto actual = Image{width, height};
    for (auto i = 0; i < lineCount; ++i)
    {
        const auto angle = i * 2.0f * pi<float> / lineCount;

        const auto start = center + Point2{{static_cast<int>(std::round(std::cos(angle) * startLength)),
                                            static_cast<int>(std::round(std::sin(angle) * startLength))}};

        const auto end = center + Point2{{static_cast<int>(std::round(std::cos(angle) * endLength)),
                                          static_cast<int>(std::round(std::sin(angle) * endLength))}};

        auto lineTracer = LineTracer{start, end};

        do
        {
            const auto position = lineTracer.position();
            if (auto& color = actual(position); color == Colors::black)
            {
                color = Colors::white;
            }
            else
            {
                color = Colors::red;
            }
        } while (lineTracer.step());

        if (auto& color = actual(start); color == Colors::white)
        {
            color = Colors::green;
        }
        else
        {
            color = Colors::red;
        }

        if (auto& color = actual(end); color == Colors::white)
        {
            color = Colors::green;
        }
        else
        {
            color = Colors::red;
        }
    }

    const auto expected = readBitmapFile("resources/dansandu/eyecandy/expected_line_tracer.bmp");
    const auto success = expected == actual;

    if (!success)
    {
        writeBitmapFile("target/actual_line_tracer.bmp", actual);
    }

    REQUIRE(success);
}
