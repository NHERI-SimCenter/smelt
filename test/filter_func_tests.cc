#include <vector>
#include <catch/catch.hpp>
#include "function_dispatcher.h"

TEST_CASE("Test window functions", "[WindowFuncs][Helpers]") {

  SECTION("Test Hann window") {
    unsigned int window_length = 23;
    auto hann_window =
        Dispatcher<std::vector<double>, double>::instance()->dispatch(
            "HannWindow", window_length);

    // This is really is simply function using std::cos function evaluated at
    // different points, so this really only tests the class construction.
    REQUIRE(hann_window[window_length / 2] == Approx(1.0).epsilon(0.01));
  }
}
