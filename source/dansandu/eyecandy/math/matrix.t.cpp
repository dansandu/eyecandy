#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <stdexcept>
#include <vector>

using dansandu::eyecandy::math::Matrix;

TEST_CASE() {

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

  SECTION("assignment to operator") {
    Matrix<double> original{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
    Matrix<double> copy = original;
    REQUIRE(std::equal(original.begin(), original.end(), copy.begin(), copy.end()));
  }

}