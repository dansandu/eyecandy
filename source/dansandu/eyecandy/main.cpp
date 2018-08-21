#include "SFML/Graphics.hpp"

#include "dansandu/eyecandy/raster/image.hpp"

using dansandu::eyecandy::raster::Image;

static constexpr auto title = "eyecandy";
static constexpr auto resolution = std::make_pair(600, 400);

int main() {
    sf::RenderWindow window(sf::VideoMode(resolution.first, resolution.second), title);
    sf::Texture texture;
    texture.create(resolution.first, resolution.second);
    sf::Sprite sprite(texture);
    Image pixels{resolution.first, resolution.second};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        texture.update(pixels.pixelArray());
        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
