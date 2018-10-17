#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"

#include <algorithm>

using dansandu::eyecandy::math::additive_identity;
using dansandu::eyecandy::math::dehomogenize;
using dansandu::eyecandy::math::dynamic;
using dansandu::eyecandy::math::Matrix;
using dansandu::eyecandy::math::slicer;

TEST_CASE("New Matrix") {

    SECTION("default construction") {
        Matrix<int, 3, 3> m;

        REQUIRE(
            std::all_of(std::begin(m), std::end(m), [](auto element) { return element == additive_identity<int>; }));
    }

    SECTION("fill construction") {
        auto fillValue = 9;
        Matrix<int, 3, 3> m{fillValue};

        REQUIRE(std::all_of(std::begin(m), std::end(m), [fillValue](auto element) { return element == fillValue; }));
    }

    SECTION("buffer construction") {
        std::vector<int> expected = {1, 2, 3, 4, 5, 6};
        Matrix<int, dynamic, dynamic> m{2, 3, expected};

        REQUIRE(std::equal(std::begin(expected), std::end(expected), std::begin(m), std::end(m)));
        REQUIRE(m.rows() == 2);
        REQUIRE(m.columns() == 3);
    }

    SECTION("from array") {
        int expected[] = {1, 2, 3, 4, 5, 6};
        Matrix<int, 2, 3> m{{{1, 2, 3}, {4, 5, 6}}};

        REQUIRE(std::equal(std::begin(expected), std::end(expected), std::begin(m), std::end(m)));
        REQUIRE(m.rows() == 2);
        REQUIRE(m.columns() == 3);
    }

    SECTION("multiplication") {
        Matrix<double, 4, 4> a{
            {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 10.0, 11.0, 12.0}, {13.0, 14.0, 15.0, 16.0}}};
        Matrix<double, 4, 4> b{
            {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}}};
        Matrix<double, 4, 4> expected = a;

        REQUIRE(expected.closeTo(a * b, 10e-10));
    }

    SECTION("dynamic multiplicaton") {
        Matrix<int, dynamic, 3> a{4, 3, {1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}};
        Matrix<int, 3, 3> b{{{2, 0, 0}, {0, 3, 0}, {0, 0, 4}}};
        Matrix<int, dynamic, 3> expected{4, 3, {2, 0, 4, 2, 3, 4, 2, 0, 0, 0, 0, 0}};

        REQUIRE(a * b == expected);
    }

    SECTION("scalar multiplication") {
        Matrix<int, 1, 10> v{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
        Matrix<int, 1, 10> expected{{5, 10, 15, 20, 25, 30, 35, 40, 45, 50}};

        REQUIRE(v * 5 == expected);
    }

    SECTION("addition") {
        Matrix<int, 2, 2> a{{{6, 2}, {10, 7}}}, b{{{12, 6}, {2, 19}}}, expected{{{18, 8}, {12, 26}}};

        REQUIRE(a + b == expected);
    }

    SECTION("subtraction") {
        Matrix<int, 2, 2> a{{{6, 2}, {10, 7}}}, b{{{12, 6}, {2, 19}}}, expected{{{-6, -4}, {8, -12}}};

        REQUIRE(a - b == expected);
    }

    SECTION("closeness for floating point values") {
        Matrix<double, 2, 3> a{{{1.06, 2.0012, 3.0}, {4.00002, 5.005, 6.0068}}}, b{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}};

        REQUIRE(a.closeTo(b, 0.1));
        REQUIRE(!a.closeTo(b, 0.01));
    }

    SECTION("closeness for integer values") {
        Matrix<int, 2, 3> a{{{1, 2, 3}, {4, 5, 6}}}, b{{{1, 2, 3}, {4, 5, 6}}};

        REQUIRE(a.closeTo(b, 0));
        REQUIRE(!a.closeTo(b, -1));
    }

    SECTION("slicing static matrix") {
        Matrix<int, 2, 2> a{{{1, 2}, {3, 4}}};

        REQUIRE(slicer<0, 1, 0, 2>::slice(a) == Matrix<int, 1, 2>{{1, 2}});
        REQUIRE(slicer<dynamic, 1, 0, 2>::slice(a, 0) == Matrix<int, 1, 2>{{1, 2}});
        REQUIRE(slicer<dynamic, 1, 0, 2>::slice(a, 1) == Matrix<int, 1, 2>{{3, 4}});
    }

    SECTION("slicing dynamic matrix") {
        Matrix<int, dynamic, 3> a{4, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}};

        REQUIRE(slicer<dynamic, 1, 0, 3>::slice(a, 3) == Matrix<int, 1, 3>{{10, 11, 12}});
        REQUIRE(slicer<dynamic, 2, 1, 2>::slice(a, 2) == Matrix<int, 2, 2>{{{8, 9}, {11, 12}}});
        REQUIRE(slicer<0, dynamic, 0, 2>::slice(a, 4) == Matrix<int, dynamic, 2>{4, 2, {1, 2, 4, 5, 7, 8, 10, 11}});
    }

    SECTION("dehomogenize") {
        Matrix<int, 2, 4> a{{{2, 4, 8, 2}, {3, 9, 27, 3}}};

        dehomogenize(a);

        REQUIRE(a == Matrix<int, 2, 4>{{{1, 2, 4, 1}, {1, 3, 9, 1}}});
    }
}
