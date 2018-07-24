#include "catch/catch.hpp"
#include "dansandu/eyecandy/geometry/sphere.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/raster/colors.hpp"
#include "dansandu/eyecandy/raster/fragment.hpp"
#include "dansandu/eyecandy/raster/image.hpp"
#include <iostream>

using dansandu::eyecandy::geometry::sphere;
using dansandu::eyecandy::math::rotationByX;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::drawMesh;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::writeBitmapFile;

TEST_CASE("Sphere") {

    SECTION("") {
        auto size = 300;
        Image image{size, size};
        auto mySphere = sphere<double>(size * 0.4, 3, 4);
        mySphere.vertices = translation<double>(size / 2.0, size / 2.0, 0.0) * mySphere.vertices;
        drawMesh(image, mySphere, Colors::turquoise);
        writeBitmapFile("resource/expected_sphere.bmp", image);
    }
}
