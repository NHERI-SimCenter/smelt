#include <iostream>
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

    filter_order = 2;
    auto hp_butter =
        Dispatcher<std::vector<std::vector<double>>, int, double>::instance()
            ->dispatch("HighPassButter", filter_order, cutoff_freq);

    std::vector<double> numerators(3);
    std::vector<double> denominators(3);

    // numerator[0] = 
    
    std::cout << "\nCoefficients:\n";
    for (auto const& val : hp_butter) {
      for (auto const& entry : val) {
    	std::cout << entry << " ";
      }
      std::cout << std::endl;
    }
    
    // // This is really is simply function using std::cos function evaluated at
    // // different points, so this really only tests the class construction.
    // REQUIRE(hann_window[window_length / 2] == Approx(1.0).epsilon(0.01));
  }
}
