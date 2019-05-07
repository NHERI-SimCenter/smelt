#include <string>
#include <vector>
#include <catch/catch.hpp>
#include "function_dispatcher.h"

TEST_CASE("Test wind velocity profile functions", "[WindProfile][Helpers]") {

  SECTION("Test ASCE exposure category based power law velocity profile") {
    std::string exposure_category = "E";
    std::vector<double> heights(4, 0.0);
    std::vector<double> wind_velocities(4);
    double karman_const = 0.4;
    double gust_speed = 30.0;

    for (unsigned int i = 0; i < heights.size(); ++i) {
      heights[i] = static_cast<double>(i);
    }

    // Should throw error for order less than 1 and greater than 12
    REQUIRE_THROWS_AS(
        (Dispatcher<double, const std::string &, const std::vector<double> &,
                    double, double, std::vector<double> &>::instance()
             ->dispatch("ExposureCategoryVel", exposure_category, heights, karman_const,
                        gust_speed, wind_velocities)),
        std::invalid_argument);

    // Check for exposure category D
    exposure_category = "D";
    auto friction_vel =
        Dispatcher<double, const std::string &, const std::vector<double> &,
                   double, double, std::vector<double> &>::instance()
            ->dispatch("ExposureCategoryVel", exposure_category, heights,
                       karman_const, gust_speed, wind_velocities);

    std::vector<double> expected_vels = {0.0, 18.582328384, 20.070024743,
                                         20.99488999};
    double expected_fric_vel = 1.263007919;

    for (unsigned int i = 0; i < wind_velocities.size(); ++i) {
      REQUIRE(wind_velocities[i] - expected_vels[i] + 1.0 ==
              Approx(1.0).epsilon(0.01));
    }
    REQUIRE(friction_vel - expected_fric_vel + 1.0 == Approx(1.0).epsilon(0.01));

    // Check for exposure category A
    exposure_category = "A";
    friction_vel =
        Dispatcher<double, const std::string &, const std::vector<double> &,
                   double, double, std::vector<double> &>::instance()
            ->dispatch("ExposureCategoryVel", exposure_category, heights,
                       karman_const, gust_speed, wind_velocities);

    expected_vels = {0.0, 4.17742995, 5.263231929, 6.024896551};
    expected_fric_vel = 2.236805764;

    for (unsigned int i = 0; i < wind_velocities.size(); ++i) {
      REQUIRE(wind_velocities[i] - expected_vels[i] + 1.0 ==
              Approx(1.0).epsilon(0.01));
    }
    REQUIRE(friction_vel - expected_fric_vel + 1.0 == Approx(1.0).epsilon(0.01));
    
  }
}
