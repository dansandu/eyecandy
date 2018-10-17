#pragma once

#include "dansandu/eyecandy/geometry/clipping.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <ostream>
#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

using dansandu::eyecandy::math::dynamic;

template<typename T>
class Mesh {
    using Matrix4 = dansandu::eyecandy::math::Matrix4<T>;
    using vertexSlicer = dansandu::eyecandy::math::slicer<dynamic, 1, 0, 3>;

public:
    using VertexMatrix = dansandu::eyecandy::math::Matrix<T, dynamic, 4>;
    using TriangleMatrix = dansandu::eyecandy::math::Matrix<size_type, dynamic, 3>;

    Mesh(VertexMatrix vertices, TriangleMatrix triangles)
        : vertices_{std::move(vertices)}, triangles_{std::move(triangles)} {}

    Mesh(Mesh&&) noexcept = default;

    Mesh(const Mesh&) = default;

    Mesh& operator=(Mesh&&) noexcept = default;

    Mesh& operator=(const Mesh&) = default;

    auto clip() {
        auto clippedVerticesAndTriangles = geometry::clip(vertices_, triangles_);
        vertices_ = std::move(clippedVerticesAndTriangles.first);
        triangles_ = std::move(clippedVerticesAndTriangles.second);
    }

    auto dehomogenize() { math::dehomogenize(vertices_); }

    auto cull() { triangles_ = geometry::cull(vertices_, triangles_); }

    auto transform(const Matrix4& matrix) {
        if (vertices_.rows())
            vertices_ = vertices_ * matrix;
    }

    template<typename Rasterizer>
    auto rasterize(Rasterizer&& rasterizer) const {
        for (auto triangleIndex = 0; triangleIndex < triangles_.rows(); ++triangleIndex)
            rasterizer(getVertex(triangleIndex, 0), getVertex(triangleIndex, 1), getVertex(triangleIndex, 2));
    }

private:
    auto getVertex(size_type triangleIndex, size_type vertexIndex) const {
        return vertexSlicer::slice(vertices_, triangles_(triangleIndex, vertexIndex));
    }

    VertexMatrix vertices_;
    TriangleMatrix triangles_;
};
}
}
}
