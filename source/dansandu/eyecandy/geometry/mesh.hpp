#pragma once

#include "Eigen/Dense"

#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

template<typename T>
struct Mesh {
    Mesh() = default;

    Mesh(const Eigen::Matrix<T, 4, Eigen::Dynamic>& vertices, const Eigen::Matrix<int, 3, Eigen::Dynamic>& triangles)
        : vertices(vertices), triangles(triangles) {}

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    Eigen::Matrix<T, 4, Eigen::Dynamic> vertices;
    Eigen::Matrix<int, 3, Eigen::Dynamic> triangles;
};
}
}
}
