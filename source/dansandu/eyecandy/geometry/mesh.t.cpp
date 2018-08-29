#include "catch/catch.hpp"
#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"

using dansandu::eyecandy::geometry::Mesh;
using dansandu::eyecandy::math::translation;

// clang-format off

TEST_CASE("Mesh") {
    auto triangle = Mesh<double>{{{-12.0,  0.0, 0.0, 2.0},
                                  {  0.0, 12.0, 0.0, 3.0},
                                  { 12.0,  0.0, 0.0, 4.0}}, {{0, 1, 2}}};
    
    SECTION("dehomogenize") {
        triangle.dehomogenize();

        REQUIRE(triangle.verticesCloseTo({{-6.0, 0.0, 0.0, 1.0},
                                          { 0.0, 4.0, 0.0, 1.0},
                                          { 3.0, 0.0, 0.0, 1.0}}, 10e-15));
    }

    SECTION("clipping") {
        triangle.clip();

        REQUIRE(triangle.verticesCloseTo({{ 3.27273, 3.27273, 0.0, 3.27273},
                                          { 3.27273, 0.00000, 0.0, 3.27273},
                                          {-2.76923, 0.00000, 0.0, 2.76923},
                                          {-2.76923, 2.76923, 0.0, 2.76923}}, 10e-5));
        REQUIRE(triangle.trianglesEqualTo({{0, 1, 2}, {0, 2, 3}}));
    }

    SECTION("transform") {
        triangle.transform(translation(11.0, 13.0, 17.0));

        REQUIRE(triangle.verticesCloseTo({{10.0, 26.0, 34.0, 2.0},
                                          {33.0, 51.0, 51.0, 3.0},
                                          {56.0, 52.0, 68.0, 4.0}}, 10e-10));
    }
}

// clang-format on
