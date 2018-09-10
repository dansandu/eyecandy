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
using dansandu::eyecandy::math::perspective;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::math::viewport;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;

TEST_CASE("Sphere") {
    SECTION("backface culling") {
        Image actual{100, 100};
        auto mesh = sphere(150.0, 8, 8);
        mesh.transform(translation(0.0, 0.0, -300.0));
        mesh.transform(perspective(1.0, 2000.0, 1.92, 1.0));
        mesh.dehomogenize();
        mesh.transform(viewport<double>(actual.width() - 1, actual.height() - 1));
        mesh.cull();
        mesh.drawWireframe(actual, Colors::magenta);

        auto match = actual == readBitmapFile("resource/expected_backface_culled_sphere.bmp");
        if (!match)
            writeBitmapFile("target/actual_backface_culled_sphere.bmp", actual);

        REQUIRE(match);
    }
}
