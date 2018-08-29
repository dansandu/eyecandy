#include "catch/catch.hpp"

#include "dansandu/eyecandy/geometry/sphere.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/colors.hpp"
#include "dansandu/eyecandy/raster/fragment.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

using dansandu::eyecandy::geometry::sphere;
using dansandu::eyecandy::math::closeTo;
using dansandu::eyecandy::math::Matrix;

TEST_CASE("Sphere") {

    SECTION("factory") {
        auto actual = sphere<double>(100.0, 3, 2);
        // clang-format off
        REQUIRE(actual.verticesCloseTo({{  0.0,  100.0,   0.0, 1.0},
                                        {-86.6,   50.0,   0.0, 1.0},
                                        { 43.3,   50.0,  75.0, 1.0},
                                        { 43.3,   50.0, -75.0, 1.0},
                                        {-86.6,  -50.0,   0.0, 1.0},
                                        { 43.3,  -50.0,  75.0, 1.0},
                                        { 43.3,  -50.0, -75.0, 1.0},
                                        {  0.0, -100.0,   0.0, 1.0}}, 0.01));

        REQUIRE(actual.trianglesEqualTo({{0, 1, 2},
                                         {2, 1, 4},
                                         {2, 4, 5},
                                         {5, 4, 7},
                                         {0, 2, 3},
                                         {3, 2, 5},
                                         {3, 5, 6},
                                         {6, 5, 7},
                                         {0, 3, 1},
                                         {1, 3, 6},
                                         {1, 6, 4},
                                         {4, 6, 7}}));
        // clang-format on
    }
}
