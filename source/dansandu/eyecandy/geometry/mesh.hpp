#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/utility/exception.hpp"

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

    auto vertex(size_type triangle, size_type index) const {
        if (triangle < 0 || triangle >= triangles.rows())
            THROW(std::invalid_argument, "cannot get the #th triangle from the mesh -- mesh only contains # triangles",
                  triangle, triangles.rows());

        if (index < 0 || index >= triangles.columns())
            THROW(std::invalid_argument,
                  "cannot get the #th vertex from the triangle -- triangle only contains # vertices", index,
                  triangles.columns());

        auto vertexIndex = triangles(triangle, index);
        if (vertexIndex < 0 || vertexIndex >= vertices.rows())
            THROW(std::invalid_argument, "cannot get the #th vertex from the mesh -- mesh only contains # vertices",
                  vertexIndex, vertices.rows());

        return vertices.row(vertexIndex);
    }

    auto triangleCount() const { return triangles.rows(); }

    math::Matrix<T> vertices;
    math::Matrix<int> triangles;
};
}
}
}
