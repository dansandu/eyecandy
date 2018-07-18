#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <stdexcept>
#include <vector>

using dansandu::eyecandy::math::Matrix;

TEST_CASE("Matrix") {

    SECTION("empty matrix construction") {
        Matrix<double> emptyMatrix;

        REQUIRE(emptyMatrix.rows() == 0);
        REQUIRE(emptyMatrix.columns() == 0);
    }

    SECTION("construction") {
        Matrix<double> matrix(2, 3, 7.0);

        REQUIRE(matrix.rows() == 2);
        REQUIRE(matrix.columns() == 3);
        REQUIRE(std::all_of(matrix.cbegin(), matrix.cend(), [](auto elem) { return elem == 7.0; }));
    }

    SECTION("bad construction") {
        REQUIRE_THROWS_AS(Matrix<double>(0, 4), std::runtime_error);
        REQUIRE_THROWS_AS(Matrix<double>(3, 0), std::runtime_error);
        REQUIRE_THROWS_AS(Matrix<double>(-2, 5), std::runtime_error);
        REQUIRE_THROWS_AS(Matrix<double>(2, -5), std::runtime_error);
    }

    SECTION("construction with initializer list") {
        Matrix<double> nullMatrix{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        Matrix<double> expected(2, 3);

        REQUIRE(nullMatrix.closeTo(expected, 0.0));
    }

    SECTION("assignment and equality") {
        Matrix<int> original{{1, 2, 3}, {4, 5, 6}};
        auto copy = original;

        REQUIRE(original.rows() == copy.rows());
        REQUIRE(original.columns() == copy.columns());
        REQUIRE(std::equal(original.begin(), original.end(), copy.begin(), copy.end()));
        REQUIRE(copy.closeTo(original, 0));
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
