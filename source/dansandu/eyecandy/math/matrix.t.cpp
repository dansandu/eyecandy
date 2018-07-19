#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <stdexcept>
#include <vector>

using dansandu::eyecandy::math::Matrix;

TEST_CASE("Matrix") {

    SECTION("closeness") {
        SECTION("for integer values") {
            Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};
            Matrix<int> sameMatrix{{1, 2, 3}, {4, 5, 6}};

            REQUIRE(sameMatrix.closeTo(matrix, 0));
            REQUIRE(!sameMatrix.closeTo(matrix, -1));
        }

        SECTION("for floating point values") {
            Matrix<double> matrix{{1.06, 2.0012, 3.0}, {4.00002, 5.005, 6.0068}};
            Matrix<double> sameMatrix{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

            REQUIRE(sameMatrix.closeTo(matrix, 0.1));
            REQUIRE(!sameMatrix.closeTo(matrix, 0.01));
        }
    }

    SECTION("empty construction") {
        Matrix<double> empty;

        REQUIRE(empty.rows() == 0);
        REQUIRE(empty.columns() == 0);
    }

    SECTION("fill construction") {
        Matrix<int> actual(2, 3, 7);
        Matrix<int> expected = {{7, 7, 7}, {7, 7, 7}};

        REQUIRE(actual.rows() == 2);
        REQUIRE(actual.columns() == 3);
        REQUIRE(expected.closeTo(actual, 0));
    }

    SECTION("bad construction") {
        REQUIRE_THROWS_AS(Matrix<double>(0, 4), std::runtime_error);
        REQUIRE_THROWS_AS(Matrix<double>(3, 0), std::runtime_error);
        REQUIRE_THROWS_AS(Matrix<double>(-2, 5), std::runtime_error);
        REQUIRE_THROWS_AS(Matrix<double>(2, -5), std::runtime_error);
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
}
