#pragma once

#include "dansandu/eyecandy/geometry/mesh.hpp"
#include "dansandu/eyecandy/raster/image.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

void drawLine(Image& image, int x0, int y0, int x1, int y1, Color color);
}
}
}
