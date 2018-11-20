#include <vector>
#include <catch/catch.hpp>
#include "function_dispatcher.h"

TEST_CASE("Test filter functions", "[FilterFuncs][Helpers]") {

  SECTION("Test highpass Butterworth filter") {
    int filter_order = 13;
    double cutoff_freq = 250.0 / 500.0;

    // Should throw error for order less than 1 and greater than 12
    REQUIRE_THROWS_AS(
        (Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
             ->dispatch("HighPassButter", filter_order, cutoff_freq)),
        std::runtime_error);
    filter_order = 0;
    REQUIRE_THROWS_AS(
        (Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
             ->dispatch("HighPassButter", filter_order, cutoff_freq)),
        std::runtime_error);

    // Check that second order filter gives expected values for coefficients
    filter_order = 2;
    auto hp_butter =
        Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
            ->dispatch("HighPassButter", filter_order, cutoff_freq);

    // Expected values based for coefficients
    std::vector<double> numerators = {0.2929, -0.5858, 0.2929};
    std::vector<double> denominators = {1.0000, 0.0, 0.1716};

    for (unsigned int i = 0; i < hp_butter[0].size(); ++i) {
      // Add one in case values are nearly zero
      REQUIRE(hp_butter[0][i] - numerators[i] + 1.0 ==
              Approx(1.0).epsilon(0.01));
      REQUIRE(hp_butter[1][i] - denominators[i] + 1.0 ==
              Approx(1.0).epsilon(0.01));
    }

    cutoff_freq = 250.0 / 1000.0;
    hp_butter =
        Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
            ->dispatch("HighPassButter", filter_order, cutoff_freq);

    numerators = {0.5690, -1.1381, 0.5690};
    denominators = {1.0000, -0.9428, 0.3333};

    for (unsigned int i = 0; i < hp_butter[0].size(); ++i) {
      // Add one in case values are nearly zero
      REQUIRE(hp_butter[0][i] - numerators[i] + 1.0 ==
              Approx(1.0).epsilon(0.01));
      REQUIRE(hp_butter[1][i] - denominators[i] + 1.0 ==
              Approx(1.0).epsilon(0.01));
    }
  }

  SECTION("Test filter impulse response calculation") {
    int filter_order = 2;
    double cutoff_freq = 250.0 / 500.0;
    int num_samples = 5;
    
    auto hp_butter =
        Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
            ->dispatch("HighPassButter", filter_order, cutoff_freq);

    auto impulse_response =
        Dispatcher<std::vector<double>, std::vector<double>,
                   std::vector<double>, int, int>::instance()
            ->dispatch("ImpulseResponse", hp_butter[0], hp_butter[1],
                       filter_order, num_samples);

    std::vector<double> expected_response = {
        0.292893218813453, -0.585786437626905, 0.242640687119285,
        0.100505063388335, -0.041630560342616};

    REQUIRE(impulse_response.size() == expected_response.size());
    for (unsigned int i = 0; i < impulse_response.size(); ++i) {
      REQUIRE(impulse_response[i] + 1.0 == Approx(expected_response[i] + 1.0).epsilon(0.1));
    }    
  }
}
