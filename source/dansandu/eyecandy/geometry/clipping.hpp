#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <iostream>

namespace dansandu {
namespace eyecandy {
namespace geometry {

template<typename T>
auto clipCode(const math::Matrix<T>& vertex) {
    auto x = vertex(0), y = vertex(1), z = vertex(2), w = vertex(3);
    return (x > w) << 5 | (x < -w) << 4 | (y > w) << 3 | (y < -w) << 2 | (z > w) << 1 | z < -w;
}

template<typename T>
Mesh<T> clip(const Mesh<T>& mesh) {
    using dansandu::eyecandy::math::Matrix;
    constexpr auto w = 3;
    std::vector<T> vertices;
    std::vector<int> triangles;
    for (auto triangle = 0; triangle < mesh.triangleCount(); ++triangle) {
        std::vector<Matrix<T>> polygon = {mesh.vertex(triangle, 0), mesh.vertex(triangle, 1), mesh.vertex(triangle, 2)},
                               clippedPolygon;
        for (auto side = -1.0; side < 2.0; side += 2.0)
            for (auto axis = 0; axis < 3; ++axis) {
                for (auto a = 0, b = 1; a < static_cast<int>(polygon.size()); b = (++a + 1) % polygon.size()) {
                    if (polygon[a](w) > side * polygon[a](axis))
                        clippedPolygon.push_back(polygon[a]);
                    auto t = (side * polygon[a](w) - polygon[a](axis)) /
                             (polygon[b](axis) - polygon[a](axis) - side * (polygon[b](w) - polygon[a](w)));
                    if (0.0 < t && t < 1.0)
                        clippedPolygon.push_back(polygon[a] + (polygon[b] - polygon[a]) * t);
                }
                polygon = std::move(clippedPolygon);
            }

        auto offset = static_cast<int>(vertices.size()) / 4;
        for (auto i = 0; i < static_cast<int>(polygon.size()); ++i) {
            vertices.insert(vertices.end(), polygon[i].begin(), polygon[i].end());
            if (i > 1)
                triangles.insert(triangles.end(), {offset, offset + i - 1, offset + i});
        }
    }
    return {{static_cast<int>(vertices.size()) / 4, 4, std::move(vertices)},
            {static_cast<int>(triangles.size()) / 3, 3, std::move(triangles)}};
}
}
}
}
