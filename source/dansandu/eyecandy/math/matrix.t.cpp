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

        REQUIRE(nullMatrix.rows() == 2);
        REQUIRE(nullMatrix.columns() == 3);
    }

    SECTION("assignment") {
        Matrix<double> original{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
        Matrix<double> copy = original;

        REQUIRE(original.rows() == copy.rows());
        REQUIRE(original.columns() == copy.columns());
        REQUIRE(std::equal(original.begin(), original.end(), copy.begin(), copy.end()));
    }

    SECTION("multiplication") {
        Matrix<int> expected{{82, 88}, {199, 214}};
        auto actual = Matrix<int>{{1, 2, 3}, {4, 5, 6}} * Matrix<int>{{11, 12}, {13, 14}, {15, 16}};

        REQUIRE(actual.rows() == expected.rows());
        REQUIRE(actual.columns() == expected.columns());
        REQUIRE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
    }

    SECTION("implace multiplication") {
        Matrix<int> expected{{82, 88}, {199, 214}};
        Matrix<int> actual = {{1, 2, 3}, {4, 5, 6}};
        actual *= Matrix<int>{{11, 12}, {13, 14}, {15, 16}};

        REQUIRE(actual.rows() == expected.rows());
        REQUIRE(actual.columns() == expected.columns());
        REQUIRE(std::equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
    }
}
