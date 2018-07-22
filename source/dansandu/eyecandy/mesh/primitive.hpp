#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

#include <cmath>
#include <stdexcept>

namespace dansandu {
namespace eyecandy {
namespace mesh {
    struct Triangle {
        int v0, v1, v3;
    };

    template<typename T>
    struct Mesh {
        Matrix<T> vertices;
        std::vector<Triangle> triangles;
    };

    template<typename T>
    auto sphere(T radius, int verticalResolution, int horizontalResolution) {
        if  (verticalResolution < 1)
            throw std::invalid_argument("vertical resolution must be one or more");

        if (horizontalResolution < 3)
            throw std::invalid_argument("horizontal resolution must be three or more");

        using dansandu::eyecandy::math::pi;
        using dansandu::eyecandy::math::Matrix;
        using dansandu::eyecandy::math::multiplicative_identity;
        
        auto n = 1;
        auto 2pi = pi<T> + pi<T>;
        auto vertexCount = verticalResolution * horizontalResolution + 2;
        Matrix<T> vertices(4, vertexCount);
        for (auto i = 1; i < verticalResolution; ++i) {
            auto verticalRadians = i * 2pi / verticalResolution;
            auto x0 = radius * std::cos(verticalRadians);
            auto y = radius * std::sin(verticalRadians);
            for (auto j = 0; j < horizontalResolution; ++j) {
                auto horizontalRadians = j * 2pi / horizontalResolution;
                vertices(0, n) = x0 * std::cos(horizontalResolution);
                vertices(1, n) = y;
                vertices(2, n) = -x0 * std::sin(horizontalResolution);
                vertices(3, n) = multiplicative_identity<T>;
                ++n;
            }
        }
        vertices(1, 0) = radius;
        vertices(1, n) = -radius;
        vertices(3, 0) = vertices(3, n) = multiplicative_identity<T>;
    
        return result;
    }
}
}
}
