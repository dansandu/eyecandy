#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <stdexcept>
#include <vector>

using dansandu::eyecandy::math::crossProduct;
using dansandu::eyecandy::math::dotProduct;
using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::math::normalized;

TEST_CASE("Matrix") {

    SECTION("closeness for integer values") {
        Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};
        Matrix<int> sameMatrix{{1, 2, 3}, {4, 5, 6}};

        REQUIRE(sameMatrix.closeTo(matrix, 0));
        REQUIRE(!sameMatrix.closeTo(matrix, -1));
    }

    SECTION("closeness for floating point values") {
        Matrix<double> matrix{{1.06, 2.0012, 3.0}, {4.00002, 5.005, 6.0068}};
        Matrix<double> sameMatrix{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

        REQUIRE(sameMatrix.closeTo(matrix, 0.1));
        REQUIRE(!sameMatrix.closeTo(matrix, 0.01));
    }

    SECTION("empty construction") {
        Matrix<double> empty;

        REQUIRE(empty.rows() == 0);
        REQUIRE(empty.columns() == 0);
    }

    SECTION("fill construction") {
        Matrix<int> actual(2, 3, 7);
        Matrix<int> expected{{7, 7, 7}, {7, 7, 7}};

        REQUIRE(actual.rows() == 2);
        REQUIRE(actual.columns() == 3);
        REQUIRE(expected.closeTo(actual, 0));
    }

    SECTION("bad construction") {
        REQUIRE_THROWS_AS(Matrix<double>(-2, 5), std::invalid_argument);
        REQUIRE_THROWS_AS(Matrix<double>(2, -5), std::invalid_argument);
    }

    SECTION("construction with initializer list") {
        Matrix<int> actual{{0, 0, 0}, {0, 0, 0}};
        Matrix<int> expected(2, 3);

        REQUIRE(actual.closeTo(expected, 0));
    }

    SECTION("copy and move semantics") {
        Matrix<int> original{{1, 2, 3}, {4, 5, 6}};

        SECTION("copy construction") {
            auto copy = original;

            REQUIRE(copy.closeTo(original, 0));
        }

        SECTION("move constrution") {
            auto copy = original;
            auto moved = std::move(copy);

            REQUIRE(copy.rows() == 0);
            REQUIRE(copy.columns() == 0);

            REQUIRE(moved.closeTo(original, 0));
        }

        SECTION("copy assignment") {
            Matrix<int> another(3, 5, 7);
            another = original;

            REQUIRE(another.closeTo(original, 0));
        }

        SECTION("move assignment") {
            Matrix<int> another(3, 5, 7);
            auto copy = original;
            another = std::move(copy);

            REQUIRE(copy.rows() == 0);
            REQUIRE(copy.columns() == 0);

            REQUIRE(another.closeTo(original, 0));
        }
    }

    SECTION("multiplication") {
        Matrix<int> expected{{82, 88}, {199, 214}};
        auto actual = Matrix<int>{{1, 2, 3}, {4, 5, 6}} * Matrix<int>{{11, 12}, {13, 14}, {15, 16}};

        REQUIRE(expected.closeTo(actual, 0));
    }

    SECTION("inplace multiplication") {
        Matrix<int> expected{{82, 88}, {199, 214}};
        Matrix<int> actual = {{1, 2, 3}, {4, 5, 6}};
        actual *= Matrix<int>{{11, 12}, {13, 14}, {15, 16}};

        REQUIRE(expected.closeTo(actual, 0));
    }

    SECTION("normalization") {
        Matrix<double> vector = {3.0, 4.0, 2.0};

        REQUIRE(normalized(vector).closeTo({0.557, 0.742, 0.371}, 1e-3));
    }

    SECTION("cross product") {
        Matrix<double> vector = {1.0, 0.0, 0.0};

        REQUIRE(crossProduct(vector, {0.0, 1.0, 0.0}).closeTo({0.0, 0.0, 1.0}, 1e-15));
    }

    SECTION("subtraction") {
        Matrix<int> a = {{6, 2}, {10, 7}}, b = {{12, 6}, {2, 19}};

        REQUIRE((a - b).closeTo({{-6, -4}, {8, -12}}, 0));
    }

    SECTION("multiplication by a scalar") {
        Matrix<int> matrix = {{0, 15}, {-4, 13}};

        REQUIRE((matrix * 7).closeTo({{0, 105}, {-28, 91}}, 0));
    }

    SECTION("row indexing") {
        Matrix<int> matrix = {{0, 1}, {2, 3}, {4, 5}};

        REQUIRE(matrix.row(0).closeTo({{0, 1}}, 0));
        REQUIRE(matrix.row(1).closeTo({{2, 3}}, 0));
        REQUIRE(matrix.row(2).closeTo({{4, 5}}, 0));
    }

    SECTION("addition") {
        Matrix<int> a = {{6, 2}, {10, 7}}, b = {{12, 6}, {2, 19}};

        REQUIRE((a + b).closeTo({{18, 8}, {12, 26}}, 0));
    }

    SECTION("dot product") { REQUIRE(dotProduct<double>({2.0, 11.0, 13.0}, {6.0, -2.0, 5.0}) == 55.0); }
}
