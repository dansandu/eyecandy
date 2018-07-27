#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"

#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

struct Triangle {
    int v0, v1, v2;
};

template<typename T>
struct Mesh {
    Mesh() = default;

    Mesh(math::Matrix<T> vertices, math::Matrix<int> triangles)
        : vertices(std::move(vertices)), triangles(std::move(triangles)) {}

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    math::Matrix<T> vertices;
    math::Matrix<int> triangles;
};
}
}
}
