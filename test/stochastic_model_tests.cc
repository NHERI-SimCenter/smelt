#include <iostream>
#include <cmath>
#include <catch/catch.hpp>
#include <Eigen/Dense>
#include "vlachos_et_al.h"

TEST_CASE("Test Vlachos et al. (2018) implementation", "[Stochastic][Seismic]") {

  stochastic::VlachosEtAl test_model(6.5, 30.0, 500.0, 30.0, 0.01, 0.2, 2, 2);

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

    auto modulating_factors = test_model.amplitude_modulating_function(
        duration, total_energy, params, times);
    std::vector<double> expected_value = {1.414213562373095, 0.756590125372443};

    REQUIRE(std::isnan(modulating_factors[0]));
    REQUIRE(modulating_factors[1] - expected_value[0] + 1.0 ==
            Approx(1.0).epsilon(0.01));
    REQUIRE(modulating_factors[2] - expected_value[1] + 1.0 ==
            Approx(1.0).epsilon(0.01));
  }

  SECTION("Test acceleration rotation") {
    std::vector<double> acceleration = {1.0};
    std::vector<double> x_accels(acceleration.size());
    std::vector<double> y_accels(acceleration.size());

    test_model.rotate_acceleration(acceleration, x_accels, y_accels);

    REQUIRE(x_accels[0] == Approx(std::sqrt(3.0)/2.0).epsilon(0.01));
    REQUIRE(y_accels[0] == Approx(1.0/2.0).epsilon(0.01));

    stochastic::VlachosEtAl test_model_2(6.5, 30.0, 500.0, 315.0);
    test_model_2.rotate_acceleration(acceleration, x_accels, y_accels);

    REQUIRE(x_accels[0] == Approx(1.0/std::sqrt(2.0)).epsilon(0.01));
    REQUIRE(y_accels[0] == Approx(-1.0/std::sqrt(2.0)).epsilon(0.01));
  }

  SECTION("Test K-T model") {
    std::vector<double> params = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    std::vector<double> frequencies = {1.0, 2.0};
    std::vector<double> hp_butter_energy = {0.5, 0.75};

    auto power_spectrum = test_model.kt_2(params, frequencies, hp_butter_energy);
    std::vector<double> expected_value = {5.63257127563669, 8.39842328684051};

    for (unsigned int i = 0; i < power_spectrum.size(); ++i) {
      REQUIRE(power_spectrum[i] == Approx(expected_value[i]).epsilon(0.01));
    }
  }

  SECTION("Test time history generation") {
    auto success = test_model.generate("TestHistory", "./test.json");
    REQUIRE(success);
  }
}
