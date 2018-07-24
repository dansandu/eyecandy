#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

void drawLine(Image& image, int x0, int y0, int x1, int y1, Color color);

template<typename T>
void drawMesh(Image& image, const geometry::Mesh<T>& mesh, Color color) {
    for (const auto& triangle : mesh.triangles) {
        drawLine(image, mesh.vertices(0, triangle.v0), mesh.vertices(1, triangle.v0), mesh.vertices(0, triangle.v1),
                 mesh.vertices(1, triangle.v1), color);
        drawLine(image, mesh.vertices(0, triangle.v0), mesh.vertices(1, triangle.v0), mesh.vertices(0, triangle.v2),
                 mesh.vertices(1, triangle.v2), color);
        drawLine(image, mesh.vertices(0, triangle.v1), mesh.vertices(1, triangle.v1), mesh.vertices(0, triangle.v2),
                 mesh.vertices(1, triangle.v2), color);
    }
}
}
}
}
