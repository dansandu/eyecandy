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
        Matrix<double> expectedVertices =
            {{  0.00, -86.60, 43.30,  43.30, -86.60,  43.30,  43.30,    0.00},
             {100.00,  50.00, 50.00,  50.00, -50.00, -50.00, -50.00, -100.00},
             {  0.00,   0.00, 75.00, -75.00,   0.00,  75.00, -75.00,    0.00},
             {  1.00,   1.00,  1.00,   1.00,   1.00,   1.00,   1.00,    1.00}};

        Matrix<int> expectedTriangles =
            {{0, 2, 2, 5, 0, 3, 3, 6, 0, 1, 1, 4},
             {1, 1, 4, 4, 2, 2, 5, 5, 3, 3, 6, 6},
             {2, 4, 5, 7, 3, 5, 6, 7, 1, 6, 4, 7}};
        // clang-format on
        REQUIRE(actual.vertices.closeTo(expectedVertices, 0.01));
        REQUIRE(actual.triangles.closeTo(expectedTriangles, 0));
    }
}
