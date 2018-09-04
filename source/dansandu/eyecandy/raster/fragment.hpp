#pragma once

#include "dansandu/eyecandy/math/point.hpp"
#include "dansandu/eyecandy/raster/line_tracer.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

template<typename Shader>
auto drawLine(math::Point a, math::Point b, Shader&& shader) {
    auto lineTracer = LineTracer{a, b};
    do {
        shader(lineTracer.position());
    } while (lineTracer.step());
}

template<typename Shader>
auto drawFlatTopTriangle(math::Point bottom, math::Point leftTop, math::Point rightTop, Shader&& shader) {
    LineTracer leftTracer{bottom, leftTop}, rightTracer{bottom, rightTop};
    for (auto y = bottom.y; y <= leftTop.y; ++y) {
        auto leftXbegin = leftTracer.position().x, leftXend = leftXbegin;
        auto rightXbegin = rightTracer.position().x, rightXend = rightXbegin;

        while (leftTracer.step() && y == leftTracer.position().y)
            leftXend = leftTracer.position().x;

        while (rightTracer.step() && y == rightTracer.position().y)
            rightXend = rightTracer.position().x;

        for (auto x = std::min(leftXbegin, leftXend); x <= std::max(rightXbegin, rightXend); ++x)
            shader(math::Point{x, y});
    }
}

template<typename Shader>
void drawTriangle(math::Point a, math::Point b, math::Point c, Shader&& shader) {
    sortPointsByYx(a, b, c);
    if (b.y == c.y)
        drawFlatTopTriangle(a, b, c, std::forward<Shader>(shader));
}
}
}
}
