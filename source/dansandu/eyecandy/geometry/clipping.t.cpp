#include "catch/catch.hpp"
#include "dansandu/eyecandy/geometry/clipping.hpp"

using dansandu::eyecandy::geometry::clip;
using dansandu::eyecandy::geometry::cull;
using dansandu::eyecandy::math::dynamic;
using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::math::size_type;

// clang-format off

TEST_CASE("Clipping") {

    SECTION("clipping") {
        Matrix<double, dynamic, 4> vertices{3, 4, {-12.0,  0.0, 0.0, 2.0,
                                                     0.0, 12.0, 0.0, 3.0, 
                                                    12.0,  0.0, 0.0, 4.0}},
                                   expectedVertices{4, 4, { 3.27273, 3.27273, 0.0, 3.27273,
                                                            3.27273, 0.00000, 0.0, 3.27273,
                                                           -2.76923, 0.00000, 0.0, 2.76923,
                                                           -2.76923, 2.76923, 0.0, 2.76923}};

        Matrix<size_type, dynamic, 3> triangles{1, 3, {0, 1, 2}}, 
                                      expectedTriangles{2, 3, {0, 1, 2,
                                                               0, 2, 3}};

        auto clippedVerticesAndTriangles = clip(vertices, triangles);

        REQUIRE(clippedVerticesAndTriangles.first.closeTo(expectedVertices, 10e-5));
        REQUIRE(clippedVerticesAndTriangles.second == expectedTriangles);
    }

    SECTION("culling") {
        Matrix<size_type, dynamic, 3> triangles{1, 3, {0, 1, 2}};

        SECTION("visible") {
            Matrix<double, dynamic, 4> vertices{3, 4, {  0.0, 12.0, 0.0, 3.0,
                                                       -12.0,  0.0, 0.0, 2.0,
                                                        12.0,  0.0, 0.0, 4.0}};

            auto culledTriangles = cull(vertices, triangles);

            REQUIRE(culledTriangles == triangles);
        }

        SECTION("back-facing") {
            Matrix<double, dynamic, 4> vertices{3, 4, {-12.0,  0.0, 0.0, 2.0,
                                                         0.0, 12.0, 0.0, 3.0,
                                                        12.0,  0.0, 0.0, 4.0}};

            auto culledTriangles = cull(vertices, triangles);

            REQUIRE(culledTriangles == Matrix<int, dynamic, 3>{});
        }
    }
}

// clang-format on
