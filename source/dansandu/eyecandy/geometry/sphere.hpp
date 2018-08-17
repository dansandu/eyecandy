#pragma once

#include "Eigen/Core"
#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

template<typename T>
Mesh<T> sphere(T radius, int yResolution, int zResolution) {
    if (yResolution < 3)
        throw std::invalid_argument("y resolution must be three or more");

    if (zResolution < 1)
        throw std::invalid_argument("z resolution must be one or more");

    using dansandu::eyecandy::math::additive_identity;
    using dansandu::eyecandy::math::multiplicative_identity;
    using dansandu::eyecandy::math::pi;

    auto vertexCount = yResolution * zResolution + 2;
    auto triangleCount = 2 * yResolution * zResolution;
    Eigen::Matrix<T, 4, Eigen::Dynamic> vertices(4, vertexCount);
    Eigen::Matrix<int, 3, Eigen::Dynamic> triangles(3, triangleCount);
    auto triangleIndex = 0;
    for (auto j = 0; j < yResolution; ++j) {
        triangles(0, triangleIndex) = 0;
        triangles(1, triangleIndex) = j + 1;
        triangles(2, triangleIndex) = (j + 1) % yResolution + 1;
        ++triangleIndex;
        auto yRadians = 2 * j * pi<T> / yResolution;
        for (auto k = 0; k < zResolution; ++k) {
            auto vertexIndex = k * yResolution + j + 1;
            auto zRadians = (k + 1) * pi<T> / (zResolution + 1) + 0.5 * pi<T>;
            vertices(0, vertexIndex) = radius * std::cos(zRadians) * std::cos(yRadians);
            vertices(1, vertexIndex) = radius * std::sin(zRadians);
            vertices(2, vertexIndex) = -radius * std::cos(zRadians) * std::sin(yRadians);
            vertices(3, vertexIndex) = multiplicative_identity<T>;

            if (k + 1 < zResolution) {
                triangles(0, triangleIndex) = k * yResolution + (j + 1) % yResolution + 1;
                triangles(1, triangleIndex) = k * yResolution + j + 1;
                triangles(2, triangleIndex) = (k + 1) * yResolution + j + 1;
                ++triangleIndex;

                triangles(0, triangleIndex) = k * yResolution + (j + 1) % yResolution + 1;
                triangles(1, triangleIndex) = (k + 1) * yResolution + j + 1;
                triangles(2, triangleIndex) = (k + 1) * yResolution + (j + 1) % yResolution + 1;
                ++triangleIndex;
            }
        }

        triangles(0, triangleIndex) = (zResolution - 1) * yResolution + (j + 1) % yResolution + 1;
        triangles(1, triangleIndex) = (zResolution - 1) * yResolution + j + 1;
        triangles(2, triangleIndex) = zResolution * yResolution + 1;
        ++triangleIndex;
    }

    vertices(0, 0) = additive_identity<T>;
    vertices(1, 0) = radius;
    vertices(2, 0) = additive_identity<T>;
    vertices(3, 0) = multiplicative_identity<T>;

    vertices(0, vertexCount - 1) = additive_identity<T>;
    vertices(1, vertexCount - 1) = -radius;
    vertices(2, vertexCount - 1) = additive_identity<T>;
    vertices(3, vertexCount - 1) = multiplicative_identity<T>;

    return Mesh<T>{vertices, triangles};
}
}
}
}
