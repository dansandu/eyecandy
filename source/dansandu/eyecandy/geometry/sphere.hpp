#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

template<typename T>
auto sphere(T radius, int yResolution, int zResolution) {
    if (yResolution < 3)
        throw std::invalid_argument{"y resolution must be three or more"};

    if (zResolution < 1)
        throw std::invalid_argument{"z resolution must be one or more"};

    using dansandu::eyecandy::math::additive_identity;
    using dansandu::eyecandy::math::dynamic;
    using dansandu::eyecandy::math::Matrix;
    using dansandu::eyecandy::math::multiplicative_identity;
    using dansandu::eyecandy::math::pi;
    using dansandu::eyecandy::math::size_type;

    auto vertexCount = yResolution * zResolution + 2;
    auto triangleCount = 2 * yResolution * zResolution;
    Matrix<T, dynamic, 4> vertices{vertexCount, 4};
    Matrix<size_type, dynamic, 3> triangles{triangleCount, 3};
    for (auto j = 0, triangleIndex = 0; j < yResolution; ++j) {
        triangles(triangleIndex, 0) = 0;
        triangles(triangleIndex, 1) = j + 1;
        triangles(triangleIndex, 2) = (j + 1) % yResolution + 1;
        ++triangleIndex;
        auto yRadians = 2 * j * pi<T> / yResolution;
        for (auto k = 0; k < zResolution; ++k) {
            auto vertexIndex = k * yResolution + j + 1;
            auto zRadians = (k + 1) * pi<T> / (zResolution + 1) + 0.5 * pi<T>;
            vertices(vertexIndex, 0) = radius * std::cos(zRadians) * std::cos(yRadians);
            vertices(vertexIndex, 1) = radius * std::sin(zRadians);
            vertices(vertexIndex, 2) = -radius * std::cos(zRadians) * std::sin(yRadians);
            vertices(vertexIndex, 3) = multiplicative_identity<T>;

            if (k + 1 < zResolution) {
                triangles(triangleIndex, 0) = k * yResolution + (j + 1) % yResolution + 1;
                triangles(triangleIndex, 1) = k * yResolution + j + 1;
                triangles(triangleIndex, 2) = (k + 1) * yResolution + j + 1;
                ++triangleIndex;

                triangles(triangleIndex, 0) = k * yResolution + (j + 1) % yResolution + 1;
                triangles(triangleIndex, 1) = (k + 1) * yResolution + j + 1;
                triangles(triangleIndex, 2) = (k + 1) * yResolution + (j + 1) % yResolution + 1;
                ++triangleIndex;
            }
        }

        triangles(triangleIndex, 0) = (zResolution - 1) * yResolution + (j + 1) % yResolution + 1;
        triangles(triangleIndex, 1) = (zResolution - 1) * yResolution + j + 1;
        triangles(triangleIndex, 2) = zResolution * yResolution + 1;
        ++triangleIndex;
    }

    vertices(0, 0) = additive_identity<T>;
    vertices(0, 1) = radius;
    vertices(0, 2) = additive_identity<T>;
    vertices(0, 3) = multiplicative_identity<T>;

    vertices(vertexCount - 1, 0) = additive_identity<T>;
    vertices(vertexCount - 1, 1) = -radius;
    vertices(vertexCount - 1, 2) = additive_identity<T>;
    vertices(vertexCount - 1, 3) = multiplicative_identity<T>;

    return std::pair<Matrix<T, dynamic, 4>, Matrix<size_type, dynamic, 3>>{std::move(vertices), std::move(triangles)};
}
}
}
}
