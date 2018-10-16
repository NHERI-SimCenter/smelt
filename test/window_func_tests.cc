#include <iostream>

#include <vector>
#include <catch/catch.hpp>
#include "factory.h"
#include "window.h"

TEST_CASE("Test window functions", "[WindowFuncs]") {

  SECTION("Test Hann window") {
    auto hann_window = Factory<signal_processing::Window>::instance()->create("HannWindow");
    unsigned int window_length = 23;   
    auto window_vals = hann_window->get_window(window_length);

    // This is really is simply function using std::cos function evaluated at
    // different points, so this really only tests the class construction.
    REQUIRE(window_vals[window_length / 2] == Approx(1.0).epsilon(0.01));
  }
}
