#include "catch/catch.hpp"
#include "dansandu/eyecandy/raster/bitmap.hpp"

using dansandu::eyecandy::raster::Colors;
using dansandu::eyecandy::raster::Image;
using dansandu::eyecandy::raster::readBitmapFile;
using dansandu::eyecandy::raster::writeBitmapFile;

TEST_CASE("Bitmap") {
    SECTION("chess board") {
        constexpr auto numberOfSquares = 4;
        constexpr auto squareSize = 10;
        Image chessboard{squareSize * numberOfSquares, squareSize * numberOfSquares};
        for (auto x = 0; x < chessboard.width(); ++x)
            for (auto y = 0; y < chessboard.height(); ++y)
                chessboard.plot(x, y, (x / squareSize + y / squareSize) % 2 ? Colors::white : Colors::turquoise);

        constexpr auto bitmapFileName = "resource/expected_chessboard.bmp";
        writeBitmapFile(bitmapFileName, chessboard);
        Image readChessboard = readBitmapFile(bitmapFileName);

        REQUIRE(chessboard == readChessboard);
    }
}
