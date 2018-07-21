#include "dansandu/eyecandy/raster/fragment.hpp"

#include <cmath>

namespace dansandu {
namespace eyecandy {
namespace raster {

void drawLine(Image& image, int x0, int y0, int x1, int y1, Color color) {
    auto width = std::abs(x1 - x0);
    auto height = std::abs(y1 - y0);
    auto dx = x0 < x1 ? 1 : -1;
    auto dy = y0 < y1 ? 1 : -1;

    if (height < width) {
        auto error = 2 * height - width;
        for (auto x = x0, y = y0; x != x1 + dx; x += dx) {
            image.plot(x, y, color);
            if (error > 0) {
                error -= 2 * width;
                y += dy;
            }
            error += 2 * height;
        }
    } else {
        auto error = height - 2 * width;
        for (auto x = x0, y = y0; y != y1 + dy; y += dy) {
            image.plot(x, y, color);
            if (error < 0) {
                error += 2 * height;
                x += dx;
            }
            error -= 2 * width;
        }
    }
}
}
}
}
