#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

namespace dansandu {
namespace eyecandy {
namespace geometry {

using dansandu::eyecandy::math::dynamic;
using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::math::RowVector4;
using dansandu::eyecandy::math::size_type;

template<typename T>
auto clip(const Matrix<T, dynamic, 4>& vertices, const Matrix<size_type, dynamic, 3>& triangles) {
    using VertexSlicer = dansandu::eyecandy::math::slicer<dynamic, 1, 0, 4>;

    auto getVertex = [&vertices, &triangles](auto triangle, auto vertexIndex) {
        return VertexSlicer::slice(vertices, triangles(triangle, vertexIndex));
    };

    std::vector<T> vertexBuffer;
    std::vector<size_type> triangleBuffer;
    for (auto triangleIndex = 0; triangleIndex < triangles.rows(); ++triangleIndex) {
        std::vector<RowVector4<T>> polygon{getVertex(triangleIndex, 0), getVertex(triangleIndex, 1),
                                           getVertex(triangleIndex, 2)},
            clippedPolygon;
        for (auto side = -1.0; side < 2.0; side += 2.0)
            for (auto axis = 0; axis < 3; ++axis) {
                for (auto a = 0, b = 1; a < static_cast<size_type>(polygon.size()); b = (++a + 1) % polygon.size()) {
                    static constexpr auto w = 3;
                    if (polygon[a](w) > side * polygon[a](axis))
                        clippedPolygon.push_back(polygon[a]);
                    auto t = (side * polygon[a](w) - polygon[a](axis)) /
                             (polygon[b](axis) - polygon[a](axis) - side * (polygon[b](w) - polygon[a](w)));
                    if (0.0 < t && t < 1.0)
                        clippedPolygon.push_back(polygon[a] + (polygon[b] - polygon[a]) * t);
                }
                polygon = std::move(clippedPolygon);
            }

        auto offset = static_cast<size_type>(vertexBuffer.size()) / 4;
        for (auto i = 0; i < static_cast<size_type>(polygon.size()); ++i) {
            vertexBuffer.insert(vertexBuffer.end(), polygon[i].begin(), polygon[i].end());
            if (i > 1)
                triangleBuffer.insert(triangleBuffer.end(), {offset, offset + i - 1, offset + i});
        }
    }
    auto vertexCount = static_cast<size_type>(vertexBuffer.size()) / vertices.columns();
    auto triangleCount = static_cast<size_type>(triangleBuffer.size()) / triangles.columns();
    return std::pair<Matrix<T, dynamic, 4>, Matrix<size_type, dynamic, 3>>{
        {vertexCount, 4, std::move(vertexBuffer)}, {triangleCount, 3, std::move(triangleBuffer)}};
}

template<typename T>
auto cull(const Matrix<T, dynamic, 4>& vertices, const Matrix<size_type, dynamic, 3>& triangles) {
    using VertexSlicer = dansandu::eyecandy::math::slicer<dynamic, 1, 0, 3>;

    auto getVertex3 = [&vertices, &triangles](auto triangleIndex, auto vertexIndex) {
        return VertexSlicer::slice(vertices, triangles(triangleIndex, vertexIndex));
    };

    std::vector<size_type> triangleBuffer;
    for (auto triangleIndex = 0; triangleIndex < triangles.rows(); ++triangleIndex) {
        auto o = getVertex3(triangleIndex, 2), u = getVertex3(triangleIndex, 0) - o,
             v = getVertex3(triangleIndex, 1) - o;
        if (u.y() * v.x() - u.x() * v.y() < dansandu::eyecandy::math::additive_identity<T>)
            triangleBuffer.insert(triangleBuffer.end(), {triangles(triangleIndex, 0), triangles(triangleIndex, 1),
                                                         triangles(triangleIndex, 2)});
    }
    auto triangleCount = static_cast<size_type>(triangleBuffer.size()) / triangles.columns();
    return Matrix<size_type, dynamic, 3>{triangleCount, triangles.columns(), std::move(triangleBuffer)};
}
}
}
}
