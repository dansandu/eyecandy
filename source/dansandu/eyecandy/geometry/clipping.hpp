#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

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
    using size_type = typename std::vector<T>::size_type;
    using dansandu::eyecandy::math::Matrix;

    std::vector<T> vertices;
    std::vector<int> triangles;
    for (auto triangle = 0; triangle < mesh.triangleCount(); ++triangle) {
        std::vector<Matrix<T>> polygon = {mesh.vertex(triangle, 0), mesh.vertex(triangle, 1), mesh.vertex(triangle, 2)};
        std::vector<Matrix<T>> newPolygon;
        for (auto side = -1.0; side < 2.0; side += 2.0)
            for (auto axis = 0; axis < 3; ++axis) {
                for (size_type a = 0, b = 1; a < polygon.size(); ++a, b = (a + 1) % polygon.size()) {
                    if (-polygon[a](3) < -side * polygon[a](axis))
                        newPolygon.push_back(polygon[a]);

                    auto t = (side * polygon[a](3) * polygon[b](3) - polygon[b](3) * polygon[a](axis)) /
                             (polygon[a](3) * polygon[b](axis) - polygon[b](3) * polygon[a](axis));

                    if (0.0 < t && t < 1.0)
                        newPolygon.push_back(polygon[a] + (polygon[b] - polygon[a]) * t);
                }
                polygon = std::move(newPolygon);
            }

        auto offset = static_cast<int>(vertices.size());
        for (auto i = 0; i < static_cast<int>(polygon.size()); ++i) {
            vertices.insert(vertices.back(), polygon[i].begin(), polygon[i].end());
            if (i > 1)
                triangles.insert(triangles.back(), {offset, offset + i - 1, offset + i});
        }
    }
    return {{static_cast<int>(vertices.size()) / 4, 4, std::move(vertices)},
            {static_cast<int>(triangles.size()) / 3, 3, std::move(triangles)}};
}
}
}
}
