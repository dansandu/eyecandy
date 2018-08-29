#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

#include <vector>

namespace dansandu {
namespace eyecandy {
namespace geometry {

using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::raster::Color;
using dansandu::eyecandy::raster::Image;

template<typename T>
struct Mesh {
    using size_type = typename Matrix<T>::size_type;

    Mesh() = default;

    Mesh(const Matrix<T>& vertices, const Matrix<int>& triangles)
        : vertices_(std::move(vertices)), triangles_(std::move(triangles)) {}

    Mesh(Mesh&&) noexcept = default;

    Mesh(const Mesh&) = default;

    Mesh& operator=(Mesh&&) noexcept = default;

    Mesh& operator=(const Mesh&) = default;

    auto dehomogenize() {
        for (auto i = 0; i < vertices_.rows(); ++i)
            for (auto j = 0; j < vertices_.columns(); ++j)
                vertices_(i, j) /= vertices_(i, vertices_.columns() - 1);
    }

    auto clip() {
        std::vector<T> vertices;
        std::vector<int> triangles;
        for (auto triangle = 0; triangle < triangles_.rows(); ++triangle) {
            std::vector<Matrix<T>> polygon = {vertex(triangle, 0), vertex(triangle, 1), vertex(triangle, 2)},
                                   clippedPolygon;
            for (auto side = -1.0; side < 2.0; side += 2.0)
                for (auto axis = 0; axis < 3; ++axis) {
                    for (auto a = 0, b = 1; a < static_cast<int>(polygon.size()); b = (++a + 1) % polygon.size()) {
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

            auto offset = static_cast<int>(vertices.size()) / 4;
            for (auto i = 0; i < static_cast<int>(polygon.size()); ++i) {
                vertices.insert(vertices.end(), polygon[i].begin(), polygon[i].end());
                if (i > 1)
                    triangles.insert(triangles.end(), {offset, offset + i - 1, offset + i});
            }
        }
        auto vertexCount = static_cast<int>(vertices.size()) / 4;
        auto triangleCount = static_cast<int>(triangles.size()) / 3;
        vertices_ = Matrix<T>(vertexCount, 4, std::move(vertices));
        triangles_ = Matrix<int>(triangleCount, 3, std::move(triangles));
    }

    auto transform(const Matrix<T>& matrix) {
        if (vertices_.rows())
            vertices_ *= matrix;
    }

    auto drawWireframe(Image& image, Color color) {
        for (auto triangle = 0; triangle < triangles_.rows(); ++triangle)
            for (auto vertex = 0; vertex < triangles_.columns(); ++vertex) {
                auto x0 = vertices_(triangles_(triangle, vertex), 0);
                auto x1 = vertices_(triangles_(triangle, vertex), 1);
                auto y0 = vertices_(triangles_(triangle, (vertex + 1) % triangles_.columns()), 0);
                auto y1 = vertices_(triangles_(triangle, (vertex + 1) % triangles_.columns()), 1);
                drawLine(image, x0, x1, y0, y1, color);
            }
    }

    auto verticesCloseTo(const Matrix<T>& matrix, T epsilon) { return vertices_.closeTo(matrix, epsilon); }

    auto trianglesEqualTo(const Matrix<int>& matrix) { return triangles_.closeTo(matrix, 0); }

private:
    auto vertex(size_type triangle, size_type vertex) const { return vertices_.row(triangles_(triangle, vertex)); }

    Matrix<T> vertices_;
    Matrix<int> triangles_;
};
}
}
}
