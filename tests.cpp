#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <vector>
#include <utility>
#include <string>
#include "operations.hpp"

// doctest documentation
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

// TEST PARSE
TEST_CASE("parse loads valid rows and skips bad rows") {
    // Raw data with outliers
    std::string raw_data =
        "x,y\n"
        "1.0,2.0\n"
        "3.0,4.0\n"
        "\n"
        ",s\n"
        "5.0,6.0,7.0\n"
        "7.0,8.0\n";

    // Parse data
    std::vector<std::pair<float, float>> data;
    parse(raw_data, data);

    // Checks
    REQUIRE(data.size() == 3);

    CHECK(data[0].first == doctest::Approx(1.0f));
    CHECK(data[0].second == doctest::Approx(2.0f));

    CHECK(data[1].first == doctest::Approx(3.0f));
    CHECK(data[1].second == doctest::Approx(4.0f));

    CHECK(data[2].first == doctest::Approx(7.0f));
    CHECK(data[2].second == doctest::Approx(8.0f));
}

// TEST STATS
TEST_CASE("stats computes basic x and y statistics") {
    // Basic Data
    std::vector<std::pair<float, float>> data = {
        {1.0f, 4.0f},
        {2.0f, 3.0f},
        {3.0f, 2.0f},
        {4.0f, 1.0f}
    };

    // Get stats
    StatsData stats_data;
    stats(data, stats_data);

    // Checks
    CHECK(stats_data.min_x == doctest::Approx(1.0f));
    CHECK(stats_data.max_x == doctest::Approx(4.0f));
    CHECK(stats_data.mean_x == doctest::Approx(2.5f));
    CHECK(stats_data.med_x == doctest::Approx(2.5f));
    CHECK(stats_data.sd_x == doctest::Approx(1.11803f).epsilon(0.001));

    CHECK(stats_data.min_y == doctest::Approx(1.0f));
    CHECK(stats_data.max_y == doctest::Approx(4.0f));
    CHECK(stats_data.mean_y == doctest::Approx(2.5f));
    CHECK(stats_data.med_y == doctest::Approx(2.5f));
    CHECK(stats_data.sd_y == doctest::Approx(1.11803f).epsilon(0.001));
}

// TEST FIT
TEST_CASE("fit recovers known parabola") {
    // Generate parabola data
    std::vector<std::pair<float, float>> data;
    float a = 2.0f;
    float b = 3.0f;
    float c = 5.0f;
    for (float x = -10; x <= 10; x += 1.0f) {
        float y = a * (x - b) * (x - b) + c;
        data.push_back({x, y});
    }

    // Get fit
    FitData fit_data;
    fit(data, fit_data);

    // Checks
    CHECK(fit_data.valid);
    CHECK(fit_data.a == doctest::Approx(a).epsilon(0.001));
    CHECK(fit_data.h == doctest::Approx(b).epsilon(0.001));
    CHECK(fit_data.k == doctest::Approx(c).epsilon(0.001));
    CHECK(fit_data.rmse == doctest::Approx(0.0).epsilon(0.001));
}

// TEST MIRROR
TEST_CASE("mirror runs on symmetric fitted parabola") {
    // Generate parabola data
    std::vector<std::pair<float, float>> data;
    float a = 2.0f;
    float b = 3.0f;
    float c = 5.0f;
    for (float x = -10; x <= 10; x += 1.0f) {
        float y = a * (x - b) * (x - b) + c;
        data.push_back({x, y});
    }

    // Get fit first
    FitData fit_data;
    fit(data, fit_data);

    // Checks
    REQUIRE(fit_data.valid);
    CHECK_NOTHROW(mirror(data, fit_data));
}