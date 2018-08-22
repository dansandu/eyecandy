#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

void drawLine(Image& image, int x0, int y0, int x1, int y1, Color color);

template<typename T>
void drawMeshWireframe(Image& image, const geometry::Mesh<T>& mesh, Color color) {
    const auto& ts = mesh.triangles;
    const auto& vs = mesh.vertices;

    for (auto triangle = 0; triangle < ts.rows(); ++triangle)
        for (auto vertex = 0; vertex < ts.columns(); ++vertex) {
            auto x0 = vs(ts(triangle, vertex), 0);
            auto x1 = vs(ts(triangle, vertex), 1);
            auto y0 = vs(ts(triangle, (vertex + 1) % ts.columns()), 0);
            auto y1 = vs(ts(triangle, (vertex + 1) % ts.columns()), 1);
            drawLine(image, x0, x1, y0, y1, color);
        }
}
}
}
}
