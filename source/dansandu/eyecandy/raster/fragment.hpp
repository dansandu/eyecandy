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
auto drawFlatTriangle(math::Point tip, math::Point flatLeft, math::Point flatRight, Shader&& shader) {
    auto dy = tip.y < flatLeft.y ? 1 : -1;
    LineTracer leftTracer{tip, flatLeft}, rightTracer{tip, flatRight};
    for (auto y = tip.y; y != flatLeft.y + dy; y += dy) {
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
        drawFlatTriangle(a, b, c, std::forward<Shader>(shader));
    else if (a.y == b.y)
        drawFlatTriangle(c, a, b, std::forward<Shader>(shader));
    else {
    }
}
}
}
}
