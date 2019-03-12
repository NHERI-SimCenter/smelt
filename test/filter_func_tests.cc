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

    // Test expected values compared to what is implemented in MATLAB code from
    // Prof. Deodatis
    filter_order = 4;
    cutoff_freq = 0.2 / (1.0 / 0.01 / 2.0);
    hp_butter =
        Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
            ->dispatch("HighPassButter", filter_order, cutoff_freq);

    numerators = {0.9837, -3.9349, 5.9023, -3.9349, 0.9837};
    denominators = {1.0000, -3.9672, 5.9020, -3.9026, 0.9677};

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

    filter_order = 4;
    cutoff_freq = 250.0 / 500.0;
    num_samples = 100;
    
    hp_butter =
        Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
            ->dispatch("HighPassButter", filter_order, cutoff_freq);

    impulse_response =
        Dispatcher<std::vector<double>, std::vector<double>,
                   std::vector<double>, int, int>::instance()
            ->dispatch("ImpulseResponse", hp_butter[0], hp_butter[1],
                       filter_order, num_samples);

    expected_response = {
        0.093980851433795,  -0.375923405735178, 0.518207706083427,
        -0.193213795657817, -0.159543182567034, 0.100548085616510,
        0.068388549153550,  -0.045456184387308, -0.030420507437456,
        0.020316853842960,  0.013577173293793,  -0.009071602095804,
        -0.006061525336678, 0.004050166903405,  0.002706237956677,
        -0.001808249804626, -0.001208234008264, 0.000807316130699,
        0.000539431397201,  -0.000360436535314, -0.000240835993405,
        0.000160921465999,  0.000107524285970,  -0.000071845430942,
        -0.000048005582188, 0.000032076304519,  0.000021432701463,
        -0.000014320873270, -0.000009568901596, 0.000006393735634,
        0.000004272157567,  -0.000002854564424, -0.000001907358969,
        0.000001274456518,  0.000000851564620,  -0.000000568997288,
        -0.000000380191833, 0.000000254036061,  0.000000169741469,
        -0.000000113417624, -0.000000075783233, 0.000000050636738,
        0.000000033834386,  -0.000000022607414, -0.000000015105791,
        0.000000010093367,  0.000000006744172,  -0.000000004506312,
        -0.000000003011021, 0.000000002011900,  0.000000001344309,
        -0.000000000898238, -0.000000000600184, 0.000000000401030,
        0.000000000267960,  -0.000000000179045, -0.000000000119634,
        0.000000000079937,  0.000000000053412,  -0.000000000035689,
        -0.000000000023847, 0.000000000015934,  0.000000000010647,
        -0.000000000007114, -0.000000000004753, 0.000000000003176,
        0.000000000002122,  -0.000000000001418, -0.000000000000947,
        0.000000000000633,  0.000000000000423,  -0.000000000000283,
        -0.000000000000189, 0.000000000000126,  0.000000000000084,
        -0.000000000000056, -0.000000000000038, 0.000000000000025,
        0.000000000000017,  -0.000000000000011, -0.000000000000008,
        0.000000000000005,  0.000000000000003,  -0.000000000000002,
        -0.000000000000001, 0.000000000000001,  0.000000000000001,
        -0.000000000000000, -0.000000000000000, 0.000000000000000,
        0.000000000000000,  -0.000000000000000, -0.000000000000000,
        0.000000000000000,  0.000000000000000,  -0.000000000000000,
        -0.000000000000000, 0.000000000000000,  0.000000000000000,
        -0.000000000000000};

    REQUIRE(impulse_response.size() == expected_response.size());
    for (unsigned int i = 0; i < impulse_response.size(); ++i) {
      REQUIRE(std::abs(impulse_response[i] - expected_response[i]) < 1E-6);
    }        
  }
}
