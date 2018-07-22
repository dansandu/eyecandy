#include "catch/catch.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"

using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;

TEST_CASE("Bitmap") {
    SECTION("chess board") {
        constexpr auto squareSize = 20;
        Image chessboard{squareSize * 8, squareSize * 8};
        for (auto x = 0; x < chessboard.width(); ++x)
            for (auto y = 0; y < chessboard.height(); ++y)
                chessboard.plot(x, y, (x / squareSize + y / squareSize) % 2 ? Colors::white : Colors::turquoise);

        constexpr auto bitmapFileName = "resource/chessboard.bmp";
        writeBitmapFile(bitmapFileName, chessboard);
        Image readChessboard = readBitmapFile(bitmapFileName);

        REQUIRE(chessboard == readChessboard);
    }
}
