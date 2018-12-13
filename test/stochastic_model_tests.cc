#include <iostream>
#include <cmath>
#include <catch/catch.hpp>
#include <Eigen/Dense>
#include "vlachos_et_al.h"

TEST_CASE("Test Vlachos et al. (2018) implementation", "[Stochastic][Seismic]") {

  stochastic::VlachosEtAl test_model(6.5, 30.0, 500.0, 0.0, 0.01, 0.2, 2, 1);

  SECTION("Test modal frequency identification") {
    std::vector<double> params = {2.0, 3.0, 4.0};
    std::vector<double> energy = {0.5, 2.5};
    auto modal_frequencies = test_model.modal_frequencies(params, energy);

    REQUIRE(modal_frequencies[0] == 4.0);
    REQUIRE(modal_frequencies[1] == -36.0);
  }

  SECTION("Test energy accumulation computation") {
    std::vector<double> params = {2.0, 0.5};
    std::vector<double> times = {0.0, 0.5, 1.0};

    auto accumulated_energy = test_model.energy_accumulation(params, times);

    std::vector<double> expected_value = {0.0, 0.556667904, 1.0};
    REQUIRE(accumulated_energy[0] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(accumulated_energy[1] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(accumulated_energy[2] - expected_value[2] + 1.0 ==
            Approx(1.0).epsilon(0.01));
  }

  SECTION("Test modal participation factor") {
    std::vector<double> params = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    std::vector<double> energy = {0.5, 1.5};

    auto participation_factors = test_model.modal_participation_factor(params, energy);
    
    std::vector<double> expected_value = {112.236819763036, 1109.33809695776};
    REQUIRE(participation_factors[0] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(participation_factors[1] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));    
  }

  SECTION("Test amplitude modulating factor") {
    std::vector<double> params = {2.0, 0.5};
    std::vector<double> times = {0.0, 1.0, 2.0};
    double duration = 2.0, total_energy = 4.0;

    auto modulating_factors = test_model.amplitude_modulating_function(duration, total_energy, params, times);

    std::vector<double> expected_value = {2.22667162014277, 1.4142135623731};
    std::cout << "\nModulating function:\n";
    for (auto const& val : modulating_factors) {
      std::cout << val << ", ";
    }
    std::cout << std::endl;
    REQUIRE(std::isnan(modulating_factors[0]));
    REQUIRE(modulating_factors[1] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(modulating_factors[2] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));
  }
}
