#pragma once

#include "dansandu/eyecandy/raster/image.hpp"

#include <stdexcept>
#include <string>

namespace dansandu {
namespace eyecandy {
namespace raster {

class BitmapReadException : public std::runtime_error {
public:
    BitmapReadException(const char* message) : runtime_error{message} {}
};

Image readBitmapFile(const std::string& path);

void writeBitmapFile(const std::string& path, Image& image);
}
}
}
