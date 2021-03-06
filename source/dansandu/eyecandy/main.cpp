#include "SFML/Graphics.hpp"

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/geometry/sphere.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/transformation.hpp"
#include "dansandu/eyecandy/raster/image.hpp"
#include "dansandu/eyecandy/raster/wireframe_rasterizer.hpp"

#include <chrono>
#include <iostream>

using dansandu::eyecandy::geometry::Mesh;
using dansandu::eyecandy::geometry::sphere;
using dansandu::eyecandy::math::lookAt;
using dansandu::eyecandy::math::Matrix4;
using dansandu::eyecandy::math::perspective;
using dansandu::eyecandy::math::pi;
using dansandu::eyecandy::math::rotationByY;
using dansandu::eyecandy::math::RowVector3;
using dansandu::eyecandy::math::scaling;
using dansandu::eyecandy::math::translation;
using dansandu::eyecandy::math::viewport;
using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::makeWireframeRasterizer;

static constexpr auto title = "eyecandy";
static constexpr auto resolution = std::make_pair(600, 400);

auto renderScene() {
    auto start = std::chrono::steady_clock::now();

    sf::RenderWindow window(sf::VideoMode(resolution.first, resolution.second), title);
    sf::Texture texture;
    texture.create(resolution.first, resolution.second);
    sf::Sprite sprite(texture);
    Image pixels{resolution.first, resolution.second};

    RowVector3<double> eye{{0.0, 0.0, 0.0}}, target{{0.0, 0.0, -300.0}}, up{{0.0, 1.0, 0.0}},
        spherePosition{{0.0, 0.0, -300.0}};
    auto sphereVerticesAndTriangles = sphere(150.0, 8, 8);
    auto rasterizer = makeWireframeRasterizer<double>(
        [&pixels](const auto& point, const auto&) { pixels.plot(point, Colors::magenta); });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {
                case sf::Keyboard::F:
                    spherePosition(0) += 10.0;
                    break;
                case sf::Keyboard::S:
                    spherePosition(0) -= 10.0;
                    break;
                case sf::Keyboard::E:
                    spherePosition(1) += 10.0;
                    break;
                case sf::Keyboard::D:
                    spherePosition(1) -= 10.0;
                    break;
                case sf::Keyboard::W:
                    spherePosition(2) += 10.0;
                    break;
                case sf::Keyboard::R:
                    spherePosition(2) -= 10.0;
                    break;
                default:
                    break;
                }
            }
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        auto sphereCopy = Mesh<double>{sphereVerticesAndTriangles.first, sphereVerticesAndTriangles.second};
        sphereCopy.transform(rotationByY(elapsed.count() % 5000 / 5000.0 * 2.0 * pi<double>) *
                             translation(spherePosition) * lookAt(eye, target, up) *
                             perspective(1.0, 2000.0, 1.92, 1.5));
        sphereCopy.clip();
        sphereCopy.dehomogenize();
        sphereCopy.transform(viewport<double>(resolution.first - 1, resolution.second - 1));
        sphereCopy.cull();
        pixels.clear();
        sphereCopy.rasterize(rasterizer);
        texture.update(pixels.pixelArray());

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

int main() {
    try {
        renderScene();
        return 0;
    } catch (std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return -1;
    }
}
