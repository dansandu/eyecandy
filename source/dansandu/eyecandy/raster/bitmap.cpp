#include "dansandu/eyecandy/raster/bitmap.hpp"
#include "dansandu/eyecandy/utility/exception.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <fstream>
#include <iterator>

namespace dansandu {
namespace eyecandy {
namespace raster {

namespace {
constexpr auto bitsPerPixel = 24;
constexpr auto horizontalResolution = 2835;
constexpr auto verticalResolution = 2835;
constexpr auto pixelArrayOffset = 54;
constexpr auto pixelArrayPadding = 4;
constexpr auto dibHeaderSize = 40;
constexpr auto colorPlanes = 1;

void wordLittleEndianSplit(uint32_t value, uint8_t* bytes) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
}

void doubleWordLittleEndianSplit(uint32_t value, uint8_t* bytes) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
}

uint32_t doubleWordLittleEndianUnion(uint8_t* bytes) {
    uint32_t value = bytes[3];
    (value <<= 8) |= bytes[2];
    (value <<= 8) |= bytes[1];
    (value <<= 8) |= bytes[0];
    return value;
}

void bitmapHeader(const Image& image, std::vector<uint8_t>& bitmap) {
    bitmap[0] = 0x42;
    bitmap[1] = 0x4D;
    doubleWordLittleEndianSplit(bitmap.size(), &bitmap[2]);
    doubleWordLittleEndianSplit(pixelArrayOffset, &bitmap[10]);
    doubleWordLittleEndianSplit(dibHeaderSize, &bitmap[14]);
    doubleWordLittleEndianSplit(image.width(), &bitmap[18]);
    doubleWordLittleEndianSplit(image.height(), &bitmap[22]);
    wordLittleEndianSplit(colorPlanes, &bitmap[26]);
    wordLittleEndianSplit(bitsPerPixel, &bitmap[28]);
    doubleWordLittleEndianSplit(bitmap.size() - pixelArrayOffset, &bitmap[34]);
    doubleWordLittleEndianSplit(horizontalResolution, &bitmap[38]);
    doubleWordLittleEndianSplit(verticalResolution, &bitmap[42]);
}

void bitmapPixelArray(Image const& image, std::vector<uint8_t>& bitmap) {
    for (auto i = 0; i < image.height(); i++) {
        for (auto j = 0; j < image.width(); j++) {
            auto color = image.color(j, i).code();
            bitmap.push_back((color >> 8) & 0xFF);
            bitmap.push_back((color >> 16) & 0xFF);
            bitmap.push_back((color >> 24) & 0xFF);
        }

        while ((bitmap.size() - pixelArrayOffset) % pixelArrayPadding)
            bitmap.push_back(0);
    }
}

void writeBinaryFile(const std::string& path, std::vector<uint8_t>& bitmap) {
    auto file = std::ofstream{path, std::ios_base::binary};
    file << std::noskipws;
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    std::copy(bitmap.begin(), bitmap.end(), std::ostream_iterator<uint8_t>(file));
    file.close();
}
}

void writeBitmapFile(const std::string& path, const Image& image) {
    auto bitmap = std::vector<uint8_t>(pixelArrayOffset, 0);
    bitmapPixelArray(image, bitmap); // set the pixel array first because we need to know the
    bitmapHeader(image, bitmap);     // file size before setting the header
    writeBinaryFile(path, bitmap);
}

Image readBitmapFile(const std::string& path) {
    auto file = std::ifstream{path, std::ios_base::binary};
    file >> std::noskipws;

    uint8_t buffer[32]{0};
    auto readBytes = [&file, &buffer](int count) mutable {
        return static_cast<bool>(file.read(reinterpret_cast<char*>(buffer), count));
    };

    if (!file)
        THROW(BitmapReadException, "file '", path, "' does not exist");

    auto firstHeaderByte = 0x42;
    auto secondHeaderByte = 0x4D;
    if (!readBytes(2) || buffer[0] != firstHeaderByte || buffer[1] != secondHeaderByte)
        THROW(BitmapReadException, "invalid magic words in header -- ", firstHeaderByte, " ", secondHeaderByte,
              " were expected instead of ", static_cast<int>(buffer[0]), " ", static_cast<int>(buffer[1]));

    if (!readBytes(16)) // skip to width at offset 18
        THROW(BitmapReadException, "bytes 2-17 are missing from the header");

    if (!readBytes(4)) // read width
        THROW(BitmapReadException, "couldn't read width from the header");

    auto readWidth = doubleWordLittleEndianUnion(buffer);
    if (readWidth > INT_MAX)
        THROW(BitmapReadException, "width is too large");
    auto width = static_cast<int>(readWidth);

    if (!readBytes(4)) // read height
        THROW(BitmapReadException, "couldn't read height from the header");
    auto readHeight = doubleWordLittleEndianUnion(buffer);
    if (readHeight > INT_MAX)
        THROW(BitmapReadException, "height is too large");
    auto height = static_cast<int>(readHeight);

    if (!readBytes(28)) // skip to pixel array at offset 54
        THROW(BitmapReadException, "bytes 26-53 are missing from the header");

    auto image = Image{width, height};
    auto pixelArraySize = 0;
    for (auto i = 0; i < height; i++) {
        for (auto j = 0; j < width; j++) {
            if (!readBytes(3))
                THROW(BitmapReadException, "color information is missing from the pixel array");

            uint32_t colorCode = buffer[2];
            (colorCode <<= 8) |= buffer[1];
            (colorCode <<= 8) |= buffer[0];
            (colorCode <<= 8) |= 0xFF;
            image.plot(j, i, Color{colorCode});
            pixelArraySize += 3;
        }
        auto padding = (pixelArraySize % 4) ? 4 - (pixelArraySize % 4) : 0;
        pixelArraySize += padding;
        if (!readBytes(padding))
            THROW(BitmapReadException, "some padding bytes are missing from the pixel array");
    }
    return image;
}
}
}
}
