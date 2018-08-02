#include "catch/catch.hpp"
#include "dansandu/eyecandy/math/matrix_view.hpp"
#include "dansandu/eyecandy/utility/string.hpp"

#include <vector>

using dansandu::eyecandy::math::columnView;
using dansandu::eyecandy::math::rowView;
using dansandu::eyecandy::utility::join;

TEST_CASE("Matrix View") {

    SECTION("nonempty matrix") {
        int array[] = {1, 2, 3, 4, 5, 6};

        SECTION("iteration by row") {
            std::vector<std::string> rows;
            for (auto row : rowView(array, 2, 3))
                rows.push_back(join(row, ", "));

            REQUIRE(join(rows, "; ") == "1, 2, 3; 4, 5, 6");
        }

        SECTION("iteration by column") {
            std::vector<std::string> columns;
            for (auto column : columnView(array, 2, 3))
                columns.push_back(join(column, ", "));

            REQUIRE(join(columns, "; ") == "1, 4; 2, 5; 3, 6");
        }

        SECTION("indexing into rows") {
            auto row = *rowView(array, 2, 3).begin();

            REQUIRE(row(0) == 1);
            REQUIRE(row(1) == 2);
            REQUIRE(row(2) == 3);
        }

        SECTION("indexing into columns") {
            auto column = *columnView(array, 2, 3).begin();

            REQUIRE(column(0) == 1);
            REQUIRE(column(1) == 4);
        }

        SECTION("deep indexing in row view") {
            auto matrix = rowView(array, 2, 3);

            REQUIRE(matrix(1)(2) == 6);
            REQUIRE(matrix(0)(1) == 2);
        }
    }

    SECTION("empty matrix") {
        int array[] = {};

        SECTION("iteration by row") {
            std::vector<std::string> rows;
            for (auto row : rowView(array, 0, 0))
                rows.push_back(join(row, ", "));

            REQUIRE(join(rows, "; ") == "");
        }

        SECTION("iteration by column") {
            std::vector<std::string> columns;
            for (auto column : columnView(array, 0, 0))
                columns.push_back(join(column, ", "));

            REQUIRE(join(columns, "; ") == "");
        }
    }

    SECTION("singleton matrix") {
        int array[] = {1};

        SECTION("iteration by row") {
            std::vector<std::string> rows;
            for (auto row : rowView(array, 1, 1))
                rows.push_back(join(row, ", "));

            REQUIRE(join(rows, "; ") == "1");
        }

        SECTION("iteration by column") {
            std::vector<std::string> columns;
            for (auto column : columnView(array, 1, 1))
                columns.push_back(join(column, ", "));

            REQUIRE(join(columns, "; ") == "1");
        }
    }
}
