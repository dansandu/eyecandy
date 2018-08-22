#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"

#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

template<typename T>
struct Mesh {
    using size_type = typename math::Matrix<T>::size_type;

    Mesh() = default;

    Mesh(const math::Matrix<T>& vertices, const math::Matrix<int>& triangles)
        : vertices(std::move(vertices)), triangles(std::move(triangles)) {}

    Mesh(Mesh&&) noexcept = default;

    Mesh(const Mesh&) = default;

    Mesh& operator=(Mesh&&) noexcept = default;

    Mesh& operator=(const Mesh&) = default;

    auto vertex(size_type triangle, size_type index) const { return vertices.row(triangles(triangle, index)); }

    auto triangleCount() const { return triangles.rows(); }

    math::Matrix<T> vertices;
    math::Matrix<int> triangles;
};
}
}
}
