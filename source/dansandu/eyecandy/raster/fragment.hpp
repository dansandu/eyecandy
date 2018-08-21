#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

void drawLine(Image& image, int x0, int y0, int x1, int y1, Color color);

template<typename T>
void drawWireframeMesh(Image& image, const geometry::Mesh<T>& mesh, Color color) {
    const auto& ts = mesh.triangles;
    const auto& vs = mesh.vertices;

    for (auto j = 0; j < ts.cols(); ++j)
        for (auto i = 0; i < ts.rows(); ++i) {
            auto x0 = vs(0, ts(i, j));
            auto x1 = vs(1, ts(i, j));
            auto y0 = vs(0, ts((i + 1) % 3, j));
            auto y1 = vs(1, ts((i + 1) % 3, j));
            drawLine(image, x0, x1, y0, y1, color);
        }
}
}
}
}
