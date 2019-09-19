#include <cmath>
#include <vector>
#include <catch2/catch.hpp>
#include <Eigen/Dense>
#include "factory.h"
#include "normal_dist.h"

TEST_CASE("Test different distribution types", "[Distributions]") {

  SECTION("Test CDF/ICDF values for normal distribution") {
    double mean = 0.0;
    double std_dev = 1.0;
    auto test_distribution =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "NormalDist", std::move(mean), std::move(std_dev));

    std::vector<double> locations = {0.0};

    auto probabilities = test_distribution->cumulative_dist_func(locations);
    auto calced_locations = test_distribution->inv_cumulative_dist_func(probabilities);
    
    REQUIRE(probabilities[0] == Approx(0.5).epsilon(0.01));
    REQUIRE(calced_locations[0] == Approx(0.0).epsilon(0.01));    
  }

  SECTION("Test CDF/ICDF values for lognormal distribution") {
    double mean = 0.0;
    double std_dev = 0.25;
    auto test_distribution =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "LognormalDist", std::move(mean), std::move(std_dev));

    std::vector<double> locations = {1.0};

    auto probabilities = test_distribution->cumulative_dist_func(locations);
    auto calced_locations = test_distribution->inv_cumulative_dist_func(probabilities);
    
    REQUIRE(probabilities[0] == Approx(0.5).epsilon(0.01));
    REQUIRE(calced_locations[0] == Approx(1.0).epsilon(0.01));
  }

  SECTION("Test CDF/ICDF values for Beta distribution") {
    double alpha = 0.5;
    double beta = 0.5;
    auto test_distribution =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "BetaDist", std::move(alpha), std::move(beta));

    std::vector<double> locations = {0.5};

    auto probabilities = test_distribution->cumulative_dist_func(locations);
    auto calced_locations = test_distribution->inv_cumulative_dist_func(probabilities);
    
    REQUIRE(probabilities[0] == Approx(0.5).epsilon(0.01));
    REQUIRE(calced_locations[0] == Approx(0.5).epsilon(0.01));    
  }

  SECTION("Test CDF/ICDF values for inverse Gaussian distribution") {
    double mean = 1.0;
    double std_dev = 1.0;
    auto test_distribution =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "InverseGaussianDist", std::move(mean), std::move(std_dev));

    std::vector<double> locations = {0.01, 0.675841, 20};

    auto probabilities = test_distribution->cumulative_dist_func(locations);
    auto calced_locations = test_distribution->inv_cumulative_dist_func(probabilities);

    REQUIRE(probabilities[0] + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(probabilities[1] == Approx(0.5).epsilon(0.01));
    REQUIRE(probabilities[2] == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_locations[0] == Approx(locations[0]).epsilon(0.01));
    REQUIRE(calced_locations[1] == Approx(locations[1]).epsilon(0.01));
    REQUIRE(calced_locations[2] == Approx(locations[2]).epsilon(0.01));
  }

  SECTION("Test CDF/ICDF values for Student's t distribution") {
    double mean = 1.0;
    double scale = 0.25;
    double dof = 1.0;
    auto test_distribution =
        Factory<stochastic::Distribution, double, double, double>::instance()
            ->create("StudentstDist", std::move(mean), std::move(scale),
                     std::move(dof));

    std::vector<double> locations = {1.0};

    auto probabilities = test_distribution->cumulative_dist_func(locations);
    auto calced_locations = test_distribution->inv_cumulative_dist_func(probabilities);

    REQUIRE(probabilities[0]== Approx(0.5).epsilon(0.01));
    REQUIRE(calced_locations[0] == Approx(locations[0]).epsilon(0.01));
  }

  SECTION("Test CDF/ICDF values for uniform distribution") {
    double lower = 0.0;
    double upper = 1.0;
    auto test_distribution =
        Factory<stochastic::Distribution, double, double>::instance()->create(
            "UniformDist", std::move(lower), std::move(upper));

    std::vector<double> locations = {0.0, 0.5, 1.0};

    auto probabilities = test_distribution->cumulative_dist_func(locations);
    auto calced_locations =
        test_distribution->inv_cumulative_dist_func(probabilities);

    REQUIRE(probabilities[0] + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_locations[0] + 1.0 == Approx(1.0).epsilon(0.01));
    REQUIRE(probabilities[1] == Approx(0.5).epsilon(0.01));
    REQUIRE(calced_locations[1] == Approx(0.5).epsilon(0.01));
    REQUIRE(probabilities[2] == Approx(1.0).epsilon(0.01));
    REQUIRE(calced_locations[2] == Approx(1.0).epsilon(0.01));
  }
}
