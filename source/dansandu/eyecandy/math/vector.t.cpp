#include "catch/catch.hpp"

#include "dansandu/eyecandy/math/matrix.hpp"

#include <algorithm>

using dansandu::eyecandy::math::closeTo;
using dansandu::eyecandy::math::ColumnVector3;
using dansandu::eyecandy::math::dotProduct;
using dansandu::eyecandy::math::magnitude;
using dansandu::eyecandy::math::matrix_cast;
using dansandu::eyecandy::math::RowVector3;
using dansandu::eyecandy::math::RowVector4;
using dansandu::eyecandy::math::toString;
using std::all_of;
using std::begin;
using std::end;
using std::equal;

TEST_CASE("RowVector") {

    SECTION("fill constructor") {
        auto fillValue = 2.5;
        RowVector4<double> actual{fillValue};

        REQUIRE(all_of(begin(actual), end(actual), [fillValue](auto elem) { return elem == fillValue; }));
    }

    SECTION("array construction") {
        double expected[] = {1.0, 2.0, 3.0, 4.0};
        RowVector4<double> actual{expected};

        REQUIRE(equal(begin(actual), end(actual), begin(expected), end(expected)));
    }

    SECTION("indexing") {
        auto x = 3.0, y = 7.0, z = 11.0, w = 13.0;
        RowVector4<double> actual{{x, y, z, w}};

        SECTION("by axis") {
            REQUIRE(actual.x() == x);
            REQUIRE(actual.y() == y);
            REQUIRE(actual.z() == z);
            REQUIRE(actual.w() == w);
        }

        SECTION("by integer") {
            REQUIRE(actual(0, 0) == x);
            REQUIRE(actual(0, 1) == y);
            REQUIRE(actual(0, 2) == z);
            REQUIRE(actual(0, 3) == w);
        }
    }

    SECTION("dot product") {
        RowVector3<int> a{{-10, 5, 7}}, b{{11, 6, -2}};

        REQUIRE(dotProduct(a, b) == -94);
    }

    SECTION("cross product") {
        RowVector3<int> a{{-10, 5, 7}}, b{{11, 6, -2}}, expected{{-52, 57, -115}};

        REQUIRE(crossProduct(a, b) == expected);
    }

    SECTION("multiplication") {
        RowVector3<int> a{{-2, 5, 0}};
        ColumnVector3<int> b{{3, -1, 5}};

        REQUIRE(a * b == dotProduct(a, b));
    }

    SECTION("unary operations") {
        RowVector3<double> actual{{3.0, 4.0, 1.0}};

        SECTION("magnitude") { REQUIRE(closeTo(magnitude(actual), 5.099, 10e-5)); }

        SECTION("normalized") {
            RowVector3<double> expected{{0.5883, 0.7844, 0.1961}};

            REQUIRE(normalized(actual).closeTo(expected, 10e-5));
        }

        SECTION("matrix cast") {
            RowVector3<int> expected{{3, 4, 1}};

            REQUIRE(matrix_cast<int>(actual) == expected);
        }
    }

    SECTION("to string") {
        SECTION("integers") {
            RowVector3<int> vector{{1, 2, 3}};

            REQUIRE(toString(vector) == "(1, 2, 3)");
        }

        SECTION("floating point") {
            RowVector3<double> vector{{1.603, 2.2067, 3.7959}};

            REQUIRE(toString(vector, 2) == "(1.60, 2.21, 3.80)");
        }
    }
}
