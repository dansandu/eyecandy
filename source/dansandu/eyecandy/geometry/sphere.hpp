#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
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

    using dansandu::eyecandy::math::Matrix;
    using dansandu::eyecandy::math::multiplicative_identity;
    using dansandu::eyecandy::math::pi;

    Mesh<T> mesh;
    auto vertexCount = yResolution * zResolution + 2;
    mesh.vertices = Matrix<T>(4, vertexCount);
    for (auto j = 0; j < yResolution; ++j) {
        mesh.triangles.pushRow({0, j + 1, (j + 1) % yResolution + 1});
        for (auto k = 0; k < zResolution; ++k) {
            auto index = k * yResolution + j + 1;
            auto yRadians = j * (pi<T> + pi<T>) / yResolution;
            auto zRadians = (k + 1) * pi<T> / (zResolution + 1);
            mesh.vertices(0, index) = radius * std::cos(zRadians + 0.5 * pi<T>) * std::cos(yRadians);
            mesh.vertices(1, index) = radius * std::sin(zRadians + 0.5 * pi<T>);
            mesh.vertices(2, index) = -radius * std::cos(zRadians + 0.5 * pi<T>) * std::sin(yRadians);
            mesh.vertices(3, index) = multiplicative_identity<T>;

            if (k + 1 < zResolution) {
                mesh.triangles.pushRow({k * yResolution + (j + 1) % yResolution + 1, k * yResolution + j + 1,
                                        (k + 1) * yResolution + j + 1});
                mesh.triangles.pushRow({k * yResolution + (j + 1) % yResolution + 1, (k + 1) * yResolution + j + 1,
                                        (k + 1) * yResolution + (j + 1) % yResolution + 1});
            }
        }
        mesh.triangles.pushRow({(zResolution - 1) * yResolution + (j + 1) % yResolution + 1,
                                (zResolution - 1) * yResolution + j + 1, zResolution * yResolution + 1});
    }

    mesh.vertices(1, 0) = radius;
    mesh.vertices(3, 0) = multiplicative_identity<T>;
    mesh.vertices(1, vertexCount - 1) = -radius;
    mesh.vertices(3, vertexCount - 1) = multiplicative_identity<T>;

    return mesh;
}
}
}
}
