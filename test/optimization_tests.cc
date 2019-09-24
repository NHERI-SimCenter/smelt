#include <cmath>
#include <functional>
#include <iostream>
#include <vector>
#include <catch2/catch.hpp>
#include <Eigen/Dense>
#include "nelder_mead.h"

TEST_CASE("Test Nelder-Mead Optimization", "[Helpers][Optimization]") {

  SECTION("Test ability to optimize Rosenbrock function") {
    optimization::NelderMead optimizer(1e-6);

    std::function<double(const std::vector<double>&)> rosenbrock =
        [](const std::vector<double> &points) -> double {
      double a = 1.0, b = 100.0;
      return std::pow(a - points[0], 2) +
             b * std::pow(points[1] - points[0] * points[0], 2);
    };

    std::vector<double> initial_point = {-10.0, -1.0};
    double delta = 0.1;

    auto calced_min_location =
        optimizer.minimize(initial_point, delta, rosenbrock);

    REQUIRE(optimizer.get_minimum() + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_min_location[0] == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_min_location[1] == Approx(1.0).epsilon(0.01));
  }

  SECTION("Test ability to optimize Rosenbrock function") {
    optimization::NelderMead optimizer(1e-6);

    std::function<double(const std::vector<double>&)> rosenbrock =
        [](const std::vector<double> &points) -> double {
      double a = 1.0, b = 100.0;
      return std::pow(a - points[0], 2) +
             b * std::pow(points[1] - points[0] * points[0], 2);
    };

    std::vector<double> initial_point = {1000.0, -2000.0};
    std::vector<double> delta = {0.1, 0.2};

    auto calced_min_location =
        optimizer.minimize(initial_point, delta, rosenbrock);

    REQUIRE(optimizer.get_minimum() + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_min_location[0] == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_min_location[1] == Approx(1.0).epsilon(0.01));
  }
}
